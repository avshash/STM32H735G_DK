#ifndef _STRING_A_PTR_H_
#define _STRING_A_PTR_H_

class String;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    StringAPtr
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class StringAPtr
{
public:
  StringAPtr (String *);
  StringAPtr (StringAPtr & source);
  ~StringAPtr ();

  operator String * ();
  String & operator* ();
  const String & operator* () const;

private:
// Not implemented.
  StringAPtr ();
  StringAPtr & operator= (StringAPtr &);

  String * m_object;
};


#endif  // _STRING_A_PTR_H_
