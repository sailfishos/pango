/* This file is taken from the FreeType (1) tree. It's been reindented
 * to roughly match Pango guidelines (in anticipation of future changes),
 * but not otherwise much altered.
 */

/****************************************************************************/
/*                                                                          */
/*  The FreeType project -- a free and portable quality TrueType renderer.  */
/*                                                                          */
/*  Copyright 1996-2000 by                                                  */
/*  D. Turner, R.Wilhelm, and W. Lemberg                                    */
/*                                                                          */
/*  arabic -- An implementation of the contextual algorithm given in the    */
/*  Unicode 2.0 book to assign the `isolated', `initial', `medial', and     */
/*  `final' properties to an input string of character codes for the Arabic */
/*  script.                                                                 */
/*                                                                          */
/*  This file is part of the FreeType project, and may only be used         */
/*  modified and distributed under the terms of the FreeType project        */
/*  license, LICENSE.TXT.  By continuing to use, modify, or distribute      */
/*  this file you indicate that you have read the license and               */
/*  understand and accept it fully.                                         */
/*                                                                          */
/* The code, like the FreeType code it is derived from is dual-licensed     */
/* under the GNU General Public License and the FreeType license. See       */
/* pango/opentype/COPYING for full details of this licensing scheme.        */
/****************************************************************************/

#include "config.h"

#include "arabic-ot.h"


/*
 *
 *     Here a table of the joining classes for characters in the range
 *     U+0620 - U+06FF, U+0750 - U+077F and U+08A0 - U+08FF.
 *
 *    The following character also has a joining class:
 *
 *     U+200D  ZERO WIDTH JOINER  -> causing
 *
 *    All other characters are given the joining class `none'.
 *
 */

static const joining_class  arabic[] =
{
  /* U+0620 */
  dual, none, right, right,
  right, right, dual, right,
  dual, right, dual, dual,
  dual, dual, dual, right,

  /* U+0630 */
  right, right, right, dual,
  dual, dual, dual, dual,
  dual, dual, dual, dual,
  dual, dual, dual, dual,

  /* U+0640 */
  causing, dual, dual, dual,
  dual, dual, dual, dual,
  right, dual, dual, transparent,
  transparent, transparent, transparent, transparent,

  /* U+0650 */
  transparent, transparent, transparent, transparent,
  transparent, transparent, transparent, transparent,
  transparent, transparent, transparent, transparent,
  transparent, transparent, transparent, none,

  /* U+0660 */
  none, none, none, none,
  none, none, none, none,
  none, none, none, none,
  none, none, dual, dual,

  /* U+0670 */
  transparent, right, right, right,
  none, right, right, right,
  dual, dual, dual, dual,
  dual, dual, dual, dual,

  /* U+0680 */
  dual, dual, dual, dual,
  dual, dual, dual, dual,
  right, right, right, right,
  right, right, right, right,

  /* U+0690 */
  right, right, right, right,
  right, right, right, right,
  right, right, dual, dual,
  dual, dual, dual, dual,

  /* U+06A0 */
  dual, dual, dual, dual,
  dual, dual, dual, dual,
  dual, dual, dual, dual,
  dual, dual, dual, dual,

  /* U+06B0 */
  dual, dual, dual, dual,
  dual, dual, dual, dual,
  dual, dual, dual, dual,
  dual, dual, dual, dual,

  /* U+06C0 */
  right, dual, dual, right,
  right, right, right, right,
  right, right, right, right,
  dual, right, dual, right,

  /* U+06D0 */
  dual, dual, right, right,
  none, right, transparent, transparent,
  transparent, transparent, transparent, transparent,
  transparent, none, none, transparent,

  /* U+06E0 */
  transparent, transparent, transparent, transparent,
  transparent, none, none, transparent,
  transparent, none, transparent, transparent,
  transparent, transparent, right, right,

  /* U+06F0 */
  none, none, none, none,
  none, none, none, none,
  none, none, dual, dual,
  dual, none, none, dual
};

static const joining_class  arabic_supplement[] =
{
  /* U+0750 */
  dual, dual, dual, dual,
  dual, dual, dual, dual,
  dual, right, right, right,
  dual, dual, dual, dual,

  /* U+0760 */
  dual, dual, dual, dual,
  dual, dual, dual, dual,
  dual, dual, dual, right,
  right, dual, dual, dual,

  /* U+0770 */
  dual, right, dual, right,
  right, dual, dual, dual,
  right, right, dual, dual,
  dual, dual, dual, dual,
};

static const joining_class  arabic_extended_a[] =
{
  /* U+08A0 */
  dual, none, dual, dual,
  dual, dual, dual, dual,
  dual, dual, right, right,
  right, none, none, none,

  /* U+08B0 */
  none, none, none, none,
  none, none, none, none,
  none, none, none, none,
  none, none, none, none,

  /* U+08C0 */
  none, none, none, none,
  none, none, none, none,
  none, none, none, none,
  none, none, none, none,

  /* U+08D0 */
  none, none, none, none,
  none, none, none, none,
  none, none, none, none,
  none, none, none, none,

  /* U+08E0 */
  none, none, none, none,
  transparent, transparent, transparent, transparent,
  transparent, transparent, transparent, transparent,
  transparent, transparent, transparent, transparent,

  /* U+08F0 */
  transparent, transparent, transparent, transparent,
  transparent, transparent, transparent, transparent,
  transparent, transparent, transparent, transparent,
  transparent, transparent, transparent, none,
};

/*     Here a table of the joining classes for characters in the range
 *     U+07C0 - U+07FF.
 *
 *     The following character also has a joining class:
 *
 *     U+200C  ZERO WIDTH NON-JOINER  -> causing
 *
 *     All other characters are given the joining class `none'.
 */
static const joining_class nko[] =
{
  /* U+07C0 */
  none, none, none, none,
  none, none, none, none,
  none, none, dual, dual,
  dual, dual, dual, dual,

  /* U+07D0 */
  dual, dual, dual, dual,
  dual, dual, dual, dual,
  dual, dual, dual, dual,
  dual, dual, dual, dual,

  /* U+07E0 */
  dual, dual, dual, dual,
  dual, dual, dual, dual,
  dual, dual, dual, transparent,
  transparent, transparent, transparent, transparent,

  /* U+07F0 */
  transparent, transparent, transparent, transparent,
  none, none, none, none,
  none, none, causing, none,
  none, none, none, none,
};

#if 0
struct cgc_
{
  FT_UShort  char_code;
  FT_UShort  glyph_index;
  FT_UShort  class;
};

typedef struct cgc_  cgc;

int  compare_cgc (const void*  a,
		  const void*  b)
{
  return (((cgc*)a)->glyph_index > ((cgc*)b)->glyph_index) ?
    1 : ((((cgc*)a)->glyph_index == ((cgc*)b)->glyph_index) ?
	  0 : -1);
}


TT_Error  Build_Arabic_Glyph_Properties (TT_CharMap        char_map,
					 TT_UShort         max_glyphs,
					 TTO_GDEFHeader**  gdef)
{
  TT_UShort  i, j, num_glyphs;

  cgc        Arabic[0x0700 - 0x0620];

  TT_UShort  glyph_indices[0x700 - 0x0620];
  TT_UShort  classes[0x700 - 0x0620];

  if (!gdef)
    return TT_Err_Invalid_Argument;

  j = 0;

  for (i = 0x0620; i < 0x0700; i++)
    {
      Arabic[j].char_code   = i;
      Arabic[j].class       = (arabic[i - 0x0620] == transparent) ?
	MARK_GLYPH : SIMPLE_GLYPH;
      Arabic[j].glyph_index = TT_Char_Index (char_map, i);
      if (Arabic[j].glyph_index)
	j++;
    }
  num_glyphs = j;

  if (!num_glyphs)
    {
      /* no Arabic font */
      *gdef = NULL;
      return TT_Err_Ok;
    }

  /* sort it */

  qsort (Arabic, num_glyphs, sizeof (cgc), compare_cgc);

  /* write it to the arrays, removing duplicates */

  glyph_indices[0] = Arabic[0].glyph_index;
  classes[0]       = Arabic[0].class;

  j = 1;

  for (i = 1; i < num_glyphs; i++)
    {
      glyph_indices[j] = Arabic[i].glyph_index;
      classes[j]       = Arabic[i].class;

      if (glyph_indices[j - 1] != glyph_indices[j])
	j++;
    }
  num_glyphs = j;

  TT_GDEF_Build_ClassDefinition (*gdef, max_glyphs, num_glyphs,
				 glyph_indices, classes);

  return TT_Err_Ok;
}
#endif

/* The joining rules as given in the Unicode 2.0 book (characters are
 * here specified as appearing in the byte stream, i.e. *not* in
 * visual order).  Joining classes are given in angle brackets, glyph
 * forms in square brackets.  Glyphs affected by a specific rule are
 * enclosed with vertical bars.
 *
 * Note: The description of the joining algorithm in the book is
 *       severely broken.  You can get a corrected version from
 *       www.unicode.org (as of 29-Jun-1999, this hasn't appeared).
 *
 *   R1: <anything1> <transparent> <anything2>
 *
 *         apply joining rules for
 *           <anything1> <anything2> -> [shape1] [shape2]
 *
 *       -> [shape1] [isolated] [shape2]
 *
 *   R2: <causing|left|dual> |<right>|
 *
 *       -> [final]
 *
 *   R3: |<left>| <causing|right|dual>
 *
 *       -> [initial]
 *
 *   R4: <causing|left|dual> |<dual>| <causing|right|dual>
 *
 *       -> [medial]
 *
 *   R5: <causing|left|dual> |<dual>| <!(causing|right|dual)>
 *
 *       -> [final]
 *
 *   R6: <!(causing|left|dual)> |<dual>| <causing|right|dual>
 *
 *       -> [initial]
 *
 *   R7: If R1-R6 fail:
 *
 *       <anything> -> [isolated]
 */

/* `direction' can be -1, 0, or 1 to indicate the last non-transparent
 * glyph, the current glyph, and the next non-transparent glyph,
 * respectively.
 */

static joining_class  Get_Joining_Class (gunichar*   string,
					 int         pos,
					 int         length,
					 int         direction,
					 gboolean    reverse)
{
  joining_class  j;


  while (1)
    {
      if (pos == 0 && direction < 0)
	return none;

      pos += direction;

      if (pos >= length)
	return none;

      if (string[pos] >= 0x0620 &&
	  string[pos] < 0x0700)
	j = arabic[string[pos] - 0x0620];
      else if (string[pos] >= 0x0750 &&
	  string[pos] < 0x0780)
	j = arabic_supplement[string[pos] - 0x0750];
      else if (string[pos] >= 0x07C0 &&
	  string[pos] < 0x0800)
	j = nko[string[pos] - 0x07C0];
      else if (string[pos] >= 0x08A0 &&
          string[pos] < 0x08FF)
	j = arabic_extended_a[string[pos] - 0x08A0];
      else if (string[pos] == 0x200D)
	return causing;
      else
	return none;

      if (!direction || j != transparent)
        {
	  if (G_UNLIKELY (reverse))
	    return j == right ? left : j == left ? right : j;
	  else
	    return j;
	}
    }
}


FT_Error  Arabic_Assign_Properties (gunichar    *string,
				    gulong      *properties,
				    int          length,
				    gboolean     reverse)
{
  joining_class  previous, current, next;
  int      i;

  if (!string || !properties || length == 0)
    return FT_Err_Invalid_Argument;

  for (i = 0; i < length; i++)
    {
      previous = Get_Joining_Class (string, i, length, -1, reverse);
      current  = Get_Joining_Class (string, i, length,  0, reverse);
      next     = Get_Joining_Class (string, i, length,  1, reverse);

      /* R1 */

      if (current == transparent)
	{
	  properties[i] |= isolated_p;
	  continue;
	}

      /* R2 */

      if (previous == causing ||
	  previous == left    ||
	  previous == dual   )
	if (current == right)
	  {
	    properties[i] |= reverse ? initial_p : final_p;
	    continue;
	  }

      /* R3 */

      if (current == left)
	if (next == causing ||
	    next == right   ||
	    next == dual   )
	  {
	    properties[i] |= reverse ? final_p : initial_p;
	    continue;
	  }

      /* R4 */

      if (previous == causing ||
	  previous == left    ||
	  previous == dual   )
	if (current == dual)
	  if (next == causing ||
	      next == right   ||
	      next == dual   )
	    {
	      properties[i] |= medial_p;
	      continue;
	    }

      /* R5 */

      if (previous == causing ||
	  previous == left    ||
	  previous == dual   )
	if (current == dual)
	  if (!(next == causing ||
		next == right   ||
		next == dual   ))
	    {
	      properties[i] |= reverse ? initial_p : final_p;
	      continue;
	    }

      /* R6 */

      if (!(previous == causing ||
	    previous == left    ||
	    previous == dual   ))
	if (current == dual)
	  if (next == causing ||
	      next == right   ||
	      next == dual   )
	    {
	      properties[i] |= reverse ? final_p : initial_p;
	      continue;
	    }

      /* R7 */

      properties[i] |= isolated_p;
    }

  return FT_Err_Ok;
}


/* End */
