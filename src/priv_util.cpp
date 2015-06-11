/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <sys/types.h>
#include <unistd.h>
#include <aul.h>
#include <pkgmgr-info.h>
#include <privilege_checker.h>

#include <types_internal.h>
#include "priv_util.h"

int ctx::privilege_util::is_allowed(const char* priv)
{
	int error;
	char app_id[256];
	error = aul_app_get_appid_bypid(getpid(), app_id, sizeof(app_id));
	IF_FAIL_RETURN_TAG(error == AUL_R_OK, ERR_OPERATION_FAILED, _E, "Getting AppId failed");

	pkgmgrinfo_appinfo_h app_info;
	error = pkgmgrinfo_appinfo_get_appinfo(app_id, &app_info);
	IF_FAIL_RETURN_TAG(error == PMINFO_R_OK, ERR_OPERATION_FAILED, _E, "Failed to get app_info");

	char *pkg_name = NULL;
	error = pkgmgrinfo_appinfo_get_pkgname(app_info, &pkg_name);
	if (error != PMINFO_R_OK || pkg_name == NULL) {
		pkgmgrinfo_appinfo_destroy_appinfo(app_info);
		_E("Failed to get package name");
		return ERR_OPERATION_FAILED;
	}

	error = privilege_checker_check_package_privilege(pkg_name, priv);
	pkgmgrinfo_appinfo_destroy_appinfo(app_info);

	_D("Privilege checking result: %d", error);

	if (error == PRIV_CHECKER_ERR_NONE) {
		return ERR_NONE;
	}

	if (error == PRIV_CHECKER_ERR_INVALID_PRIVILEGE) {
		return ERR_PERMISSION_DENIED;
	}

	return ERR_OPERATION_FAILED;
}
