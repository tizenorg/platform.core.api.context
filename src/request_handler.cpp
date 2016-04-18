/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
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

/* TODO: Remove this request_handler. All APIs and the testsuite should use DBusClient directly. */

#include <string>
#include <map>

#include <Types.h>
#include <ScopeMutex.h>
#include "DBusClient.h"
#include "request_handler.h"

using namespace ctx;

class DBusClientListenerImpl : public IDBusClientListener {
public:
	DBusClientListenerImpl() {}
	~DBusClientListenerImpl() {}
	void setCb(std::string subject, request_handler::subject_response_cb cb);
	void onPublish(std::string subject, int reqId, int error, Json event);
private:
	std::map<std::string, request_handler::subject_response_cb> __callbackMap;
};

void DBusClientListenerImpl::setCb(std::string subject, request_handler::subject_response_cb cb)
{
	__callbackMap[subject] = cb;
}

void DBusClientListenerImpl::onPublish(std::string subject, int reqId, int error, Json event)
{
	auto it = __callbackMap.find(subject);
	IF_FAIL_VOID_TAG(it != __callbackMap.end(), _W, "Unregistered subject");
	it->second(subject.c_str(), reqId, error, event);
}

static DBusClientListenerImpl __dbusListener;
static DBusClient __dbusClient;

SO_EXPORT int ctx::request_handler::subscribe(const char* subject, ctx::Json* option, int* req_id, ctx::Json* request_result)
{
	return __dbusClient.subscribe(subject, option ? *option : NULL, req_id, request_result);
}

SO_EXPORT int ctx::request_handler::unsubscribe(const char* subject, int req_id)
{
	return __dbusClient.unsubscribe(subject, req_id);
}

SO_EXPORT int ctx::request_handler::read(const char* subject, ctx::Json* option, int* req_id, ctx::Json* request_result)
{
	return __dbusClient.read(subject, option ? *option : NULL, req_id, request_result);
}

SO_EXPORT int ctx::request_handler::read_sync(const char* subject, ctx::Json* option, int* req_id, ctx::Json* data_read)
{
	return __dbusClient.readSync(subject, option ? *option : NULL, req_id, data_read);
}

SO_EXPORT int ctx::request_handler::write(const char* subject, ctx::Json* data)
{
	return __dbusClient.write(subject, *data);
}

SO_EXPORT int ctx::request_handler::write_with_reply(const char* subject, ctx::Json* data, ctx::Json* request_result)
{
	return __dbusClient.write(subject, *data, request_result);
}

SO_EXPORT int ctx::request_handler::is_supported(const char* subject)
{
	return __dbusClient.isSupported(subject);
}

SO_EXPORT bool ctx::request_handler::register_callback(const char* subject, subject_response_cb callback)
{
	__dbusListener.setCb(subject, callback);
	__dbusClient.addListener(subject, &__dbusListener);
	return true;
}
