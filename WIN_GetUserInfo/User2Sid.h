#pragma once
#include <sddl.h>
#include <Windows.h>


bool user2sid(LPWSTR csUsername, PSID Sid);