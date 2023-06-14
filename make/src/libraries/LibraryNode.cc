#include "common.h"
#include "LibraryNode.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    LibraryNode
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LibraryNode::LibraryNode (Library & parent, const String & path)
: Library (path),
  m_parent (parent)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    LibraryNode
// @method:   destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LibraryNode::~LibraryNode ()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    LibraryNode
// @method:   appendSourceParams
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
LibraryNode::appendSourceParams (QueueString & params_list) const
{
  m_parent.appendSourceParams (params_list);
  appendNodeParams (params_list);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    LibraryNode
// @method:   addGlobalParam
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
LibraryNode::addGlobalParam (const String & param_name, const String & param_value)
{
  char parsed_param[128];
  const char * c_param_value = param_value.cStr ();

  uint32_t value_len = param_value.stringLen ();
  uint32_t target_offset = 0;
  for (uint32_t offset = 0; offset < value_len; offset++)
  {
    if (c_param_value[offset] == '%')
    {
      if (c_param_value[offset + 1] == '%')
      {
        parsed_param[target_offset] = '%';
        target_offset++;
      }
      else
      {
        if (memcmp (c_param_value + offset, "%_LIBRARY_PATH%", 15) != 0)
        {
          VERIFY_INPUT_VALID (false, "Unknown directive.");
        }
        memcpy (parsed_param + target_offset, getPath ().cStr (), getPath ().stringLen ());

        fprintf (stdout, "Append path '%s'.\n", getPath ().cStr ());
        target_offset += getPath().stringLen ();
        offset += 14;
      }
    }
    else
    {
      parsed_param[target_offset] = c_param_value[offset];
      target_offset++;
    }

    INTERNAL_ASSERT (offset < 127);
  }
  parsed_param[target_offset] = 0;

  String str_param_value (parsed_param);

  m_parent.addGlobalParam (param_name, str_param_value);
}

