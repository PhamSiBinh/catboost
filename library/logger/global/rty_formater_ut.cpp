#include "rty_formater.h"

#include <library/unittest/registar.h>

namespace {
    const TStringBuf SampleISO8601("2017-07-25T19:26:09.894000+03:00");
    const TStringBuf SampleRtyLog("2017-07-25 19:26:09.894 +0300");
}

SIMPLE_UNIT_TEST_SUITE(NLoggingImplTest) {
    SIMPLE_UNIT_TEST(TestTLocalTimeSToStream) {
        NLoggingImpl::TLocalTimeS lt(TInstant::ParseIso8601(SampleISO8601));
        TStringStream ss;
        ss << lt;
        UNIT_ASSERT_EQUAL(ss.Str(), SampleRtyLog);
    }
    SIMPLE_UNIT_TEST(TestTLocalTimeSToString) {
        NLoggingImpl::TLocalTimeS lt(TInstant::ParseIso8601(SampleISO8601));
        UNIT_ASSERT_EQUAL(TString(lt), SampleRtyLog);
    }
    SIMPLE_UNIT_TEST(TestTLocalTimeSAddLeft) {
        NLoggingImpl::TLocalTimeS lt(TInstant::ParseIso8601(SampleISO8601));
        TStringBuf suffix("suffix");
        UNIT_ASSERT_EQUAL(lt + suffix, TString(SampleRtyLog) + suffix);
    }
    SIMPLE_UNIT_TEST(TestTLocalTimeSAddRight) {
        NLoggingImpl::TLocalTimeS lt(TInstant::ParseIso8601(SampleISO8601));
        TString prefix("prefix");
        UNIT_ASSERT_EQUAL(prefix + lt , prefix + SampleRtyLog);
    }
}
