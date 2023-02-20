#pragma once

#define S2DE_NO_IMPL() S2DE::Core::Utils::Logger::Warning("No function implementation %s", __func__);

#define S2DE_FATAL_ERROR(message) S2DE::Core::Utils::Logger::Fatal("%s\nTechnical information:\nLine:%d\nFile:%s\nin Function: %s" ,message , __LINE__, __FILE__ , __func__); 

// TODO: Remove fatal and add debug break
#define S2DE_ASSERT(a, d) do \
{ \
	if (!(a)) \
	{ \
		S2DE::Core::Utils::Logger::Fatal("Assertion failed %s\n File:%s\n Line:%d Description:%s", #a, __FILE__, __LINE__, d); \
	} \
} \
while (0) \

#define S2DE_VERIFY(a, d) do \
{ \
	if (!(a)) \
	{ \
		S2DE::Core::Utils::Logger::Error("Verify failed %s\n File:%s\n Line:%d Description:%s", #a, __FILE__, __LINE__, d); \
		return false; \
	} \
} \
while (0) \

#define S2DE_CHECK(a, d) do \
{ \
	if (!(a)) \
	{ \
		S2DE::Core::Utils::Logger::Error("Check failed %s\n File:%s\n Line:%d Description:%s", #a, __FILE__, __LINE__, d); \
	} \
} \
while (0) \

#define S2DE_ASSERT_HR(a, d) do \
{ \
	if (!S2DE::Core::Utils::Logger::CheckHR((a), true)) \
	{ \
		S2DE::Core::Utils::Logger::Fatal("Assertion failed %s\n File:%s\n Line:%d Description:%s", #a, __FILE__, __LINE__, d); \
	} \
} \
while (0) \

#define S2DE_CHECK_HR(a, d) do \
{ \
	if (!S2DE::Core::Utils::Logger::CheckHR((a), true)) \
	{ \
		S2DE::Core::Utils::Logger::Warning("Check failed %s\n File:%s\n Line:%d Description:%s", #a, __FILE__, __LINE__, d); \
	} \
} \
while (0) \

#define S2DE_VERIFY_HR(a, d) do \
{ \
	if (!S2DE::Core::Utils::Logger::CheckHR((a), true)) \
	{ \
		S2DE::Core::Utils::Logger::Error("Verify failed %s\n File:%s\n Line:%d Description:%s", #a, __FILE__, __LINE__, d); \
		return false; \
	} \
} \
while (0) \

// "Check" is a safe debug checking macro (HRESULT)
#define Check_HR(a, d) S2DE_CHECK_HR(a, d)
// "Verify" is a safe debug checking macro, with return (HRESULT)
#define Verify_HR(a, d) S2DE_VERIFY_HR(a, d)
// "Assert" is a debug checking macro, when it's failed it's throw fatal error (HRESULT)
#define Assert_HR(a, d) S2DE_ASSERT_HR(a, d)

// "Check" is a safe debug checking macro 
#define Check(a, d)  S2DE_CHECK(a, d)
// "Verify" is a safe debug checking macro, with return 
#define Verify(a, d) S2DE_VERIFY(a, d)
// "Assert" is a debug checking macro, when it's failed it's throw fatal error
#define Assert(a, d) S2DE_ASSERT(a, d)