/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
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

#include <glib.h>
#include <string>
#include <types_internal.h>
#include "priv_util.h"

int ctx::privilege_util::is_allowed(const char* priv)
{
	/* TODO: Re-implement using Cynara */
#if 0
	IF_FAIL_RETURN_TAG(priv, ERR_OPERATION_FAILED, _E, "Invalid parameter");

	char *subject = NULL;
	int ret = smack_new_label_from_self(&subject);
	IF_FAIL_RETURN_TAG(ret == 0 && subject != NULL, ERR_OPERATION_FAILED, _E, "Getting smack label failed");

	std::string priv_name = "privilege::tizen::";
	priv_name += priv;
	ret = smack_have_access(subject, priv_name.c_str(), "rw");
	g_free(subject);

	if (ret == 1)
		return ERR_NONE;

	if (ret == 0)
		return ERR_PERMISSION_DENIED;

	return ERR_OPERATION_FAILED;
#endif
	return ERR_NONE;
}
