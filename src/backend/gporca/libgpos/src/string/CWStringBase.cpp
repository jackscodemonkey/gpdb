//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2010 Greenplum, Inc.
//
//	@filename:
//		CWStringBase.cpp
//
//	@doc:
//		Implementation of the base abstract wide character string class
//---------------------------------------------------------------------------

#include "gpos/string/CWStringBase.h"

#include "gpos/base.h"
#include "gpos/common/clibwrapper.h"
#include "gpos/string/CWStringConst.h"

using namespace gpos;

const WCHAR CWStringBase::m_empty_wcstr = GPOS_WSZ_LIT('\0');

//---------------------------------------------------------------------------
//	@function:
//		CWStringBase::Copy
//
//	@doc:
//		Creates a deep copy of the string
//
//---------------------------------------------------------------------------
CWStringConst *
CWStringBase::Copy(CMemoryPool *mp) const
{
	return GPOS_NEW(mp) CWStringConst(mp, GetBuffer());
}

//---------------------------------------------------------------------------
//	@function:
//		CWStringBase::IsValid
//
//	@doc:
//		Checks if the string is properly NULL-terminated
//
//---------------------------------------------------------------------------
bool
CWStringBase::IsValid() const
{
	return (Length() == GPOS_WSZ_LENGTH(GetBuffer()));
}

//---------------------------------------------------------------------------
//	@function:
//		CWStringBase::operator ==
//
//	@doc:
//		Equality operator on strings
//
//---------------------------------------------------------------------------
BOOL
CWStringBase::operator==(const CWStringBase &str) const
{
	return Equals(&str);
}


//---------------------------------------------------------------------------
//	@function:
//		CWStringBase::Length()
//
//	@doc:
//		Returns the length of the string in number of wide characters,
//		not counting the terminating '\0'
//
//---------------------------------------------------------------------------
ULONG
CWStringBase::Length() const
{
	return m_length;
}

//---------------------------------------------------------------------------
//	@function:
//		CWStringBase::Equals
//
//	@doc:
//		Checks whether the string is byte-wise equal to another string
//
//---------------------------------------------------------------------------
BOOL
CWStringBase::Equals(const CWStringBase *str) const
{
	GPOS_ASSERT(nullptr != str);
	return Length() == str->Length() &&
		   0 == clib::Wcsncmp(GetBuffer(), str->GetBuffer(), Length());
}

//---------------------------------------------------------------------------
//	@function:
//		CWStringBase::IsEmpty
//
//	@doc:
//		Checks whether the string is empty
//
//---------------------------------------------------------------------------
BOOL
CWStringBase::IsEmpty() const
{
	return (0 == Length());
}

//---------------------------------------------------------------------------
//	@function:
//		CWStringBase::Find
//
//	@doc:
//		Returns the index of the first occurrence of a character, -1 if not found
//
//---------------------------------------------------------------------------
INT
CWStringBase::Find(WCHAR wc) const
{
	const WCHAR *w_str = GetBuffer();
	const ULONG length = Length();

	for (ULONG i = 0; i < length; i++)
	{
		if (wc == w_str[i])
		{
			return i;
		}
	}

	return -1;
}


//---------------------------------------------------------------------------
//	@function:
//		CWStringBase::HasEscapedCharAt
//
//	@doc:
//		Checks if a character is escaped
//
//---------------------------------------------------------------------------
BOOL
CWStringBase::HasEscapedCharAt(ULONG offset) const
{
	GPOS_ASSERT(!IsEmpty());
	GPOS_ASSERT(Length() > offset);

	const WCHAR *w_str_buffer = GetBuffer();

	for (ULONG i = offset; i > 0; i--)
	{
		// check for escape character
		if (GPOS_WSZ_LIT('\\') != w_str_buffer[i - 1])
		{
			if (0 == ((offset - i) & ULONG(1)))
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}

	// reached beginning of string
	if (0 == (offset & ULONG(1)))
	{
		return false;
	}
	else
	{
		return true;
	}
}

//---------------------------------------------------------------------------
//	@function:
//		CWStringBase::CountOccurrencesOf
//
//	@doc:
//		Count how many times the character appears in string
//
//---------------------------------------------------------------------------
ULONG
CWStringBase::CountOccurrencesOf(const WCHAR wc) const
{
	ULONG occurrences = 0;
	ULONG length = Length();
	const WCHAR *buf = GetBuffer();

	for (ULONG i = 0; i < length; i++)
	{
		if (wc == buf[i] && !HasEscapedCharAt(i))
		{
			occurrences++;
		}
	}
	return occurrences;
}
// EOF
