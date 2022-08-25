#pragma once

#include "Common.h"

class CtorDtorPrinter
{
public:
	CtorDtorPrinter(const char* name);
	~CtorDtorPrinter();

private:
	const char* _name;
};

template <typename T>
class ObjWrap : public CtorDtorPrinter
{
public:
	ObjWrap(T* obj) : CtorDtorPrinter("wrapper"), _obj(obj) {}
	virtual ~ObjWrap() {}

	T* operator ->() { return _obj; }

private:
	T* _obj;
};

class ScopeObj : public CtorDtorPrinter
{
public:
	ScopeObj() : CtorDtorPrinter("object"), _x(1) {}

	int X() { return _x; }

private:
	int _x;
};

class ObjAccess : public CtorDtorPrinter
{
public:
	ObjAccess() : CtorDtorPrinter("accessor") {}

	ObjWrap<ScopeObj> operator ->() { return ObjWrap(&obj); }

	ScopeObj& Obj() { return obj; }

private:
	ScopeObj obj;
};

void TestScope();
