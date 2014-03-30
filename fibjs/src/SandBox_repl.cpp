/*
 * SandBox_cmd.cpp
 *
 *  Created on: Mar 30, 2014
 *      Author: lion
 */

#include "SandBox.h"

#include "ifs/global.h"
#include "ifs/process.h"
#include "ifs/console.h"
#include "ifs/utils.h"

#include "ifs/Function.h"

#include <log4cpp/Category.hh>

namespace fibjs
{

void repl_command(std::string &line)
{
    if (!qstrcmp(line.c_str(), ".help"))
    {
        asyncLog(log4cpp::Priority::INFO,
                 ".exit   Exit the repl\n"
                 ".help   Show repl options\n"
                 ".info   Show fibjs build information"
                );
        return;
    }

    if (!qstrcmp(line.c_str(), ".exit"))
        process_base::exit(0);

    if (!qstrcmp(line.c_str(), ".info"))
    {
        v8::Local<v8::Object> o;

        utils_base::buildInfo(o);
        console_base::dir(o);
        return;
    }
}

result_t SandBox::repl()
{
    v8::Local<v8::Context> context(v8::Context::New (isolate));
    v8::Context::Scope context_scope(context);

    v8::Local<v8::Object> glob = context->Global();
    glob->SetHiddenValue(v8::String::NewFromUtf8(isolate, "SandBox"), wrap());

    // clone global function
    fibjs::global_base::class_info().Attach(glob);

    // clone Function.start
    fibjs::Function_base::class_info().Attach(
        glob->Get(v8::String::NewFromUtf8(isolate, "Function"))->ToObject()->GetPrototype()->ToObject());

    // module.id
    v8::Local<v8::String> strFname = v8::String::NewFromUtf8(isolate, "repl",
                                     v8::String::kNormalString, 4);
    glob->SetHiddenValue(v8::String::NewFromUtf8(isolate, "id"), strFname);

    std::string buf;
    v8::Local<v8::Value> v, v1;

    while (true)
    {
        if (!v.IsEmpty() && !v->IsUndefined())
        {
            v8::String::Utf8Value s(v);

            if (*s)
            {
                std::string str(COLOR_GREY);
                str += *s;
                str += COLOR_NORMAL;

                asyncLog(log4cpp::Priority::INFO, str.c_str());
            }
            v = v1;
        }

        std::string line;
        console_base::ac_readLine(buf.empty() ? "> " : " ... ", line);
        if (line.empty())
            continue;

        if (line[0] == '.')
        {
            repl_command(line);
            continue;
        }

        buf += line;
        buf.append("\n", 1);

        {
            v8::Local<v8::Script> script;
            v8::TryCatch try_catch;

            script = v8::Script::Compile(
                         v8::String::NewFromUtf8(isolate, buf.c_str(),
                                                 v8::String::kNormalString, (int) buf.length()),
                         strFname);
            if (script.IsEmpty())
            {
                v8::String::Utf8Value exception(try_catch.Exception());
                if (*exception && qstrcmp(*exception, "SyntaxError: Unexpected end of input"))
                {
                    buf.clear();
                    ReportException(try_catch, 0);
                }
                continue;
            }

            buf.clear();

            v = script->Run();
            if (v.IsEmpty())
                ReportException(try_catch, 0);
        }
    }

    return 0;
}

}