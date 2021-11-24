#include "pch.h"

namespace Timestamp
{
	/// <summary>
	/// Writes the current time in epoch seconds as an encrypted cipher to a file.
	/// </summary>
	/// <param name="timestamp_path">where to write the cipher</param>
	/// <returns>true if could write, false if the file already exists</returns>
	__declspec(dllexport) bool write_timestamp(std::string timestamp_path);

	/// <summary>
	/// Authenticates and descrypts the timestamp cipher in the file.
	/// </summary>
	/// <param name="timestamp_path">where the timestamp file is</param>
	/// <returns>the timestamp in epoch seconds, or 0 if the cipher in the file was tampered with</returns>
	__declspec(dllexport) uint64_t read_timestamp(std::string timestamp_path);

	/// <summary>
	/// Read the timestamp in the file and check for expiry.
	/// </summary>
	/// <param name="timestamp_path">where the timestamp file is</param>
	/// <param name="expires_after">how many seconds the software trial lasts for</param>
	/// <returns>
	/// true if the software is still in trial, false if the timestamp
	/// was tampered with (could not auuthenticate) or if the file could not be found
	/// </returns>
	__declspec(dllexport) bool check_expiry(std::string timestamp_path, uint64_t expires_after);
}


