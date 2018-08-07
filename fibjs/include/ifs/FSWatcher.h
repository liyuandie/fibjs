/***************************************************************************
 *                                                                         *
 *   This file was automatically generated using idlc.js                   *
 *   PLEASE DO NOT EDIT!!!!                                                *
 *                                                                         *
 ***************************************************************************/

#ifndef _FSWatcher_base_H_
#define _FSWatcher_base_H_

/**
 @author Leo Hoo <lion@9465.net>
 */

#include "../object.h"
#include "EventEmitter.h"

namespace fibjs {

class EventEmitter_base;

class FSWatcher_base : public EventEmitter_base {
    DECLARE_CLASS(FSWatcher_base);

public:
    // FSWatcher_base
    static result_t _new(v8::Local<v8::Array> datas, obj_ptr<FSWatcher_base>& retVal, v8::Local<v8::Object> This = v8::Local<v8::Object>());

public:
    template <typename T>
    static void __new(const T& args);

public:
    static void s__new(const v8::FunctionCallbackInfo<v8::Value>& args);
};
}

namespace fibjs {
inline ClassInfo& FSWatcher_base::class_info()
{
    static ClassData s_cd = {
        "FSWatcher", false, s__new, NULL,
        0, NULL, 0, NULL, 0, NULL, 0, NULL, NULL, NULL,
        &EventEmitter_base::class_info()
    };

    static ClassInfo s_ci(s_cd);
    return s_ci;
}

inline void FSWatcher_base::s__new(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CONSTRUCT_INIT();
    __new(args);
}

template <typename T>
void FSWatcher_base::__new(const T& args)
{
    obj_ptr<FSWatcher_base> vr;

    METHOD_NAME("new FSWatcher()");
    CONSTRUCT_ENTER();

    METHOD_OVER(1, 1);

    ARG(v8::Local<v8::Array>, 0);

    hr = _new(v0, vr, args.This());

    CONSTRUCT_RETURN();
}
}

#endif
