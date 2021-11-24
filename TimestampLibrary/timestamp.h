#include "pch.h"

namespace Timestamp
{
	__declspec(dllexport) bool write_timestamp(std::string timestamp_path);
	__declspec(dllexport) uint64_t read_timestamp(std::string timestamp_path);
	__declspec(dllexport) bool check_expiry(std::string timestamp_path, std::chrono::seconds expires_after);
}


