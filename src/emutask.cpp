#include "emutask.h"

void EmuTask::SetVM(HSQUIRRELVM v)
{
    m_vm = v;
}

bool EmuTask::Get()
{
    sq_pushroottable(m_vm);
    sq_pushstring(m_vm, _SC("g_emu_task"), -1);
    if (SQ_FAILED(sq_get(m_vm, -2))) {
        sq_pop(m_vm, 1);
        return false;
    }

    if (sq_gettype(m_vm, -1) != OT_INSTANCE)
    {
        sq_pop(m_vm, 2);
        return false;
    }

    return true;
}

bool EmuTask::Get(const SQChar *name, SQObjectType type)
{
    if (!Get()) return false;

    sq_pushstring(m_vm, name, -1);
    sq_get(m_vm, -2);

    if (type != OT_NULL && sq_gettype(m_vm, -1) != type)
    {
        sq_pop(m_vm, 2);
        return false;
    }

    return true;
}

bool EmuTask::GetClosure(const SQChar *name)
{
    if (!Get()) return false;

    sq_pushstring(m_vm, name, -1);
    sq_get(m_vm, -2);

    if (sq_gettype(m_vm, -1) != OT_CLOSURE && sq_gettype(m_vm, -1) != OT_NATIVECLOSURE)
    {
        sq_pop(m_vm, 2);
        return false;
    }

    return true;
}

bool EmuTask::SetBool(const SQChar *name, SQBool value)
{
    if (!GetClosure(name)) return false;

    sq_push(m_vm, -2);
    sq_pushbool(m_vm, value);
    SQRESULT res = sq_call(m_vm, 2, false, false);
    sq_pop(m_vm, 2);

    if (SQ_SUCCEEDED(res)) return true;
    return false;
}

bool EmuTask::SetInteger(const SQChar *name, SQInteger value)
{
    if (!GetClosure(name)) return false;

    sq_push(m_vm, -2);
    sq_pushinteger(m_vm, value);
    SQRESULT res = sq_call(m_vm, 2, false, false);
    sq_pop(m_vm, 2);

    if (SQ_SUCCEEDED(res)) return true;
    return false;
}

bool EmuTask::SetSmoothing(SQBool enable)
{
    // g_emu_task.setSmoothing(g_emu_task, enable);
    return SetBool(_SC("setSmoothing"), enable);
}

bool EmuTask::SetScanline(SQBool enable)
{
    // g_emu_task.setScanline(g_emu_task, enable);
    return SetBool(_SC("setScanline"), enable);
}
