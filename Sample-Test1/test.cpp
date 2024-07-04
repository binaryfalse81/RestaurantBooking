#include "pch.h"
#include "../Project1/BookingScheduler.cpp"

using namespace std;

class SunDaySCD : public BookingScheduler
{
public:
	SunDaySCD(int cph) : BookingScheduler{ cph }
	{
	}

	time_t getNow() override
	{
		return getTime(2021, 3, 28, 17, 0);
	}
private:
	time_t getTime(int y, int m, int d, int h, int min)
	{
		tm result = { 0, min, h, d, m - 1, y - 1900, 0, 0, -1 };
		
		return mktime(&result);
	}
};

class MonDaySCD : public BookingScheduler
{
public:
	MonDaySCD(int cph) : BookingScheduler{ cph }
	{
	}

	time_t getNow() override
	{
		return getTime(2024, 6, 3, 17, 0);
	}
private:
	time_t getTime(int y, int m, int d, int h, int min)
	{
		tm result = { 0, min, h, d, m - 1, y - 1900, 0, 0, -1 };
		
		return mktime(&result);
	}
};


class TdMailSender : public MailSender
{
public:
	void sendMail(Schedule* s) override
	{
		cout << "�׽�Ʈ �� SMS Sendor ����" << endl;
		cntSendMethodIsCalled++;
	}

	int getSendMethodCalledCnt()
	{
		return cntSendMethodIsCalled;
	}

private:
	int cntSendMethodIsCalled = 0;
};

class TdSmsSender : public SmsSender
{
public:
	void send(Schedule* s) override
	{
		cout << "�׽�Ʈ �� SMS Sendor ����" << endl;
		sendMethodIsCalled = true;
	}

	bool checkSendMethodCalled()
	{
		return sendMethodIsCalled;
	}

private:
	bool sendMethodIsCalled = false;
};

class BookingFixture : public testing::Test
{
protected:
	void SetUp() override
	{
		NOT_ON_THE_HOUR = getTime(2021, 3, 26, 9, 5);
		ON_THE_HOUR = getTime(2021, 3, 26, 9, 0);
		bookingScheduler.setSmsSender(&testSmsSender);
		bookingScheduler.setMailSender(&testMailSender);
	}
public:
	tm getTime(int y, int m, int d, int h, int min)
	{
		tm result = {0, min, h, d, m - 1, y - 1900, 0, 0, -1};
		mktime(&result);
		return result;
	}

	tm plusHour(tm base, int hour)
	{
		base.tm_hour += 1;
		mktime(&base);
		return base;
	}

	tm NOT_ON_THE_HOUR;
	tm ON_THE_HOUR;
	Customer CUSTOMER{ "Fake name", "010-1234-5678" };
	Customer CUSTOMER_W_MAIL{ "Fake name", "010-1234-5678", "test@naver.com"};
	const int UNDER_CAPACITY = 1;
	const int CAPACITY_PER_HOUR = 3;
	BookingScheduler bookingScheduler{ CAPACITY_PER_HOUR };
	TdSmsSender testSmsSender;
	TdMailSender testMailSender;
};

TEST_F(BookingFixture, ������_���ÿ���_�����ϴ�_���ð�_�ƴѰ��_����Ұ�) {
	Schedule* schedule = new Schedule{ NOT_ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };

	//act
	EXPECT_THROW({
		bookingScheduler.addSchedule(schedule);
		}, std::runtime_error);

	//assert
	//expected runtime exception
}

TEST_F(BookingFixture, ������_���ÿ���_�����ϴ�_������_���_���డ��) {
	Schedule* schedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };

	//act
	bookingScheduler.addSchedule(schedule);

	//assert
	EXPECT_EQ(true, bookingScheduler.hasSchedule(schedule));
}

TEST_F(BookingFixture, �ð��뺰_�ο�������_�ִ�_����_�ð��뿡_Capacity_�ʰ���_���_���ܹ߻�) {
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER };
	bookingScheduler.addSchedule(schedule);

	//act
	try
	{
		Schedule* newSchedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };
		bookingScheduler.addSchedule(newSchedule);
		FAIL();
	}
	catch (runtime_error& e)
	{
		//assert
		EXPECT_EQ(string{ e.what() }, string{ "Number of people is over restaurant capacity per hour" });
	}
}

TEST_F(BookingFixture, �ð��뺰_�ο�������_�ִ�_����_�ð��밡_�ٸ���_Capacity_���־_������_�߰�_����) {
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER };
	bookingScheduler.addSchedule(schedule);

	tm diff = plusHour(ON_THE_HOUR, 1);
	Schedule* newSchedule = new Schedule{ diff, UNDER_CAPACITY, CUSTOMER };
	bookingScheduler.addSchedule(newSchedule);

	EXPECT_EQ(true, bookingScheduler.hasSchedule(schedule));
	EXPECT_EQ(true, bookingScheduler.hasSchedule(newSchedule));
}

TEST_F(BookingFixture, ����Ϸ��_SMS��_������_�߼�) {
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER };

	bookingScheduler.addSchedule(schedule);

	EXPECT_EQ(true, testSmsSender.checkSendMethodCalled());
}

TEST_F(BookingFixture, �̸�����_����_��쿡��_�̸���_�̹߼�) {
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER };
	bookingScheduler.addSchedule(schedule);
	EXPECT_EQ(0, testMailSender.getSendMethodCalledCnt());
}

TEST_F(BookingFixture, �̸�����_�ִ�_��쿡��_�̸���_�߼�) {
	Schedule* schedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER_W_MAIL };
	bookingScheduler.addSchedule(schedule);
	EXPECT_EQ(1, testMailSender.getSendMethodCalledCnt());
}

TEST_F(BookingFixture, ���糯¥��_�Ͽ�����_���_����Ұ�_����ó��) {
	BookingScheduler* bs = new SunDaySCD{ CAPACITY_PER_HOUR };
	try
	{
		Schedule* schedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER_W_MAIL };
		bs->addSchedule(schedule);
		FAIL();
	}
	catch (runtime_error& e)
	{
		//assert
		EXPECT_EQ(string{ e.what() }, string{ "Booking system is not available on sunday" });
	}
}

TEST_F(BookingFixture, ���糯¥��_�Ͽ�����_�ƴѰ��_���డ��) {
	BookingScheduler* bs = new MonDaySCD{ CAPACITY_PER_HOUR };

	Schedule* schedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER_W_MAIL };
	bs->addSchedule(schedule);

	EXPECT_EQ(true, bs->hasSchedule(schedule));
}