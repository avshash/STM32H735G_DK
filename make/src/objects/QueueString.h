#ifndef _QUEUE_STRING_H_
#define _QUEUE_STRING_H_

#include "String.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    QueueString
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class QueueString
{
public:
  QueueString ();
  ~QueueString ();

  void enqueue (const char *);
  void enqueue (const String &);
  void enqueue (const QueueString &);

  uint32_t queueSize () const;
  const String & getMember (uint32_t index) const;

private:
  void verifySpace (uint32_t additional_strings_count);

  uint32_t m_strings_count;
  uint32_t m_strings_size;
  String ** m_strings;
};

#endif  // _QUEUE_STRING_H_
