/* -*- mode: C; c-file-style: "gnu" -*- */
/* xdgmimealias.c: Private file.  mmappable caches for mime data
 *
 * More info can be found at http://www.freedesktop.org/standards/
 *
 * Copyright (C) 2005  Matthias Clasen <mclasen@redhat.com>
 * Copyright (C) 2007 Peter Miller
 *
 * Licensed under the Academic Free License version 2.0
 * Or under the following terms:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <fnmatch.h>
#include <assert.h>

#include <netinet/in.h> /* for ntohl/ntohs */

#ifdef HAVE_MMAP
#include <sys/mman.h>
#else
#warning Building xdgmime without MMAP support. Binary "mime.info" cache files will not be used.
#endif

#include <sys/stat.h>
#include <sys/types.h>

#include "xdgmimecache.h"
#include "xdgmimeint.h"

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef	FALSE
#define	FALSE	(0)
#endif

#ifndef	TRUE
#define	TRUE	(!FALSE)
#endif

#ifndef _O_BINARY
#define _O_BINARY 0
#endif

#ifndef MAP_FAILED
#define MAP_FAILED ((void *) -1)
#endif

#define MAJOR_VERSION 1
#define MINOR_VERSION 0

struct _XdgMimeCache
{
  int ref_count;

  size_t  size;
  char   *buffer;
};

#define GET_UINT16(cache,offset) (ntohs(*(xdg_uint16_t*)((cache) + (offset))))
#define GET_UINT32(cache,offset) (ntohl(*(xdg_uint32_t*)((cache) + (offset))))

XdgMimeCache *
_xdg_mime_cache_ref (XdgMimeCache *cache)
{
  cache->ref_count++;
  return cache;
}

void
_xdg_mime_cache_unref (XdgMimeCache *cache)
{
  cache->ref_count--;

  if (cache->ref_count == 0)
    {
#ifdef HAVE_MMAP
      munmap (cache->buffer, cache->size);
#endif
      free (cache);
    }
}

XdgMimeCache *
_xdg_mime_cache_new_from_file (const char *file_name)
{
  XdgMimeCache *cache = NULL;

#ifdef HAVE_MMAP
  int fd = -1;
  struct stat st;
  char *buffer = NULL;

  /* Open the file and map it into memory */
  fd = open (file_name, O_RDONLY|_O_BINARY, 0);

  if (fd < 0)
    return NULL;

  if (fstat (fd, &st) < 0 || st.st_size < 4)
    goto done;

  buffer = (char *) mmap (NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);

  if (buffer == MAP_FAILED)
    goto done;

  /* Verify version */
  if (GET_UINT16 (buffer, 0) != MAJOR_VERSION ||
      GET_UINT16 (buffer, 2) != MINOR_VERSION)
    {
      munmap (buffer, st.st_size);

      goto done;
    }

  cache = (XdgMimeCache *) malloc (sizeof (XdgMimeCache));
  cache->ref_count = 1;
  cache->buffer = buffer;
  cache->size = st.st_size;

 done:
  if (fd != -1)
    close (fd);

#endif  /* HAVE_MMAP */

  return cache;
}

static int
cache_magic_matchlet_compare_to_data (XdgMimeCache *cache,
				      xdg_uint32_t  offset,
				      const void   *data,
				      size_t        len)
{
  xdg_uint32_t range_start = GET_UINT32 (cache->buffer, offset);
  xdg_uint32_t range_length = GET_UINT32 (cache->buffer, offset + 4);
  xdg_uint32_t data_length = GET_UINT32 (cache->buffer, offset + 12);
  xdg_uint32_t data_offset = GET_UINT32 (cache->buffer, offset + 16);
  xdg_uint32_t mask_offset = GET_UINT32 (cache->buffer, offset + 20);

  xdg_uint32_t i;
  xdg_uint32_t j;

  for (i = range_start; i <= range_start + range_length; i++)
    {
      int valid_matchlet = TRUE;

      if (i + data_length > len)
	return FALSE;

      if (mask_offset)
	{
	  for (j = 0; j < data_length; j++)
	    {
	      if ((((unsigned char *)cache->buffer)[data_offset + j] & ((unsigned char *)cache->buffer)[mask_offset + j]) !=
		  ((((unsigned char *) data)[j + i]) & ((unsigned char *)cache->buffer)[mask_offset + j]))
		{
		  valid_matchlet = FALSE;
		  break;
		}
	    }
	}
      else
	{
	  for (j = 0; j < data_length; j++)
	    {
	      if (((unsigned char *)cache->buffer)[data_offset + j] != ((unsigned char *) data)[j + i])
		{
		  valid_matchlet = FALSE;
		  break;
		}
	    }
	}

      if (valid_matchlet)
	return TRUE;
    }

  return FALSE;
}

static int
cache_magic_matchlet_compare (XdgMimeCache *cache,
			      xdg_uint32_t  offset,
			      const void   *data,
			      size_t        len)
{
  xdg_uint32_t n_children = GET_UINT32 (cache->buffer, offset + 24);
  xdg_uint32_t child_offset = GET_UINT32 (cache->buffer, offset + 28);

  if (cache_magic_matchlet_compare_to_data (cache, offset, data, len))
    {
      xdg_uint32_t i;

      if (n_children == 0)
	return TRUE;

      for (i = 0; i < n_children; i++)
	{
	  if (cache_magic_matchlet_compare (cache, child_offset + 32 * i,
					    data, len))
	    return TRUE;
	}
    }

  return FALSE;
}

static const char *
cache_magic_compare_to_data (XdgMimeCache *cache,
			     xdg_uint32_t  offset,
			     const void   *data,
			     size_t        len,
			     int          *prio)
{
  xdg_uint32_t priority = GET_UINT32 (cache->buffer, offset);
  xdg_uint32_t mimetype_offset = GET_UINT32 (cache->buffer, offset + 4);
  xdg_uint32_t n_matchlets = GET_UINT32 (cache->buffer, offset + 8);
  xdg_uint32_t matchlet_offset = GET_UINT32 (cache->buffer, offset + 12);

  xdg_uint32_t i;

  for (i = 0; i < n_matchlets; i++)
    {
      if (cache_magic_matchlet_compare (cache, matchlet_offset + i * 32,
					data, len))
	{
	  *prio = priority;

	  return cache->buffer + mimetype_offset;
	}
    }

  return NULL;
}

static const char *
cache_magic_lookup_data (XdgMimeCache *cache,
			 const void   *data,
			 size_t        len,
			 int          *prio,
			 const char   *mime_types[],
			 int           n_mime_types)
{
  xdg_uint32_t list_offset;
  xdg_uint32_t n_entries;
  xdg_uint32_t offset;

  xdg_uint32_t j;
  int n;

  *prio = 0;

  list_offset = GET_UINT32 (cache->buffer, 24);
  n_entries = GET_UINT32 (cache->buffer, list_offset);
  offset = GET_UINT32 (cache->buffer, list_offset + 8);

  for (j = 0; j < n_entries; j++)
    {
      const char *match;

      match = cache_magic_compare_to_data (cache, offset + 16 * j,
					   data, len, prio);
      if (match)
	return match;
      else
	{
	  xdg_uint32_t mimetype_offset;
	  const char *non_match;

	  mimetype_offset = GET_UINT32 (cache->buffer, offset + 16 * j + 4);
	  non_match = cache->buffer + mimetype_offset;

	  for (n = 0; n < n_mime_types; n++)
	    {
	      if (mime_types[n] &&
		  xdg_mime_mime_type_equal (mime_types[n], non_match))
		mime_types[n] = NULL;
	    }
	}
    }

  return NULL;
}

static const char *
cache_alias_lookup (const char *alias)
{
  const char *ptr;
  int i, min, max, mid, cmp;

  for (i = 0; _xdg_mime_caches[i]; i++)
    {
      XdgMimeCache *cache = _xdg_mime_caches[i];
      xdg_uint32_t list_offset = GET_UINT32 (cache->buffer, 4);
      xdg_uint32_t n_entries = GET_UINT32 (cache->buffer, list_offset);
      xdg_uint32_t offset;

      min = 0;
      max = n_entries - 1;
      while (max >= min)
	{
	  mid = (min + max) / 2;

	  offset = GET_UINT32 (cache->buffer, list_offset + 4 + 8 * mid);
	  ptr = cache->buffer + offset;
	  cmp = strcmp (ptr, alias);

	  if (cmp < 0)
	    min = mid + 1;
	  else if (cmp > 0)
	    max = mid - 1;
	  else
	    {
	      offset = GET_UINT32 (cache->buffer, list_offset + 4 + 8 * mid + 4);
	      return cache->buffer + offset;
	    }
	}
    }

  return NULL;
}

static int
cache_glob_lookup_literal (const char *file_name,
			   const char *mime_types[],
			   int         n_mime_types)
{
  const char *ptr;
  int i, min, max, mid, cmp;

  (void)n_mime_types;
  for (i = 0; _xdg_mime_caches[i]; i++)
    {
      XdgMimeCache *cache = _xdg_mime_caches[i];
      xdg_uint32_t list_offset = GET_UINT32 (cache->buffer, 12);
      xdg_uint32_t n_entries = GET_UINT32 (cache->buffer, list_offset);
      xdg_uint32_t offset;

      min = 0;
      max = n_entries - 1;
      while (max >= min)
	{
	  mid = (min + max) / 2;

	  offset = GET_UINT32 (cache->buffer, list_offset + 4 + 8 * mid);
	  ptr = cache->buffer + offset;
	  cmp = strcmp (ptr, file_name);

	  if (cmp < 0)
	    min = mid + 1;
	  else if (cmp > 0)
	    max = mid - 1;
	  else
	    {
	      offset = GET_UINT32 (cache->buffer, list_offset + 4 + 8 * mid + 4);
	      mime_types[0] = (const char *)(cache->buffer + offset);

	      return 1;
	    }
	}
    }

  return 0;
}

static int
cache_glob_lookup_fnmatch (const char *file_name,
			   const char *mime_types[],
			   int         n_mime_types)
{
  const char *mime_type;
  const char *ptr;

  int i, n;

  n = 0;
  for (i = 0; _xdg_mime_caches[i]; i++)
    {
      XdgMimeCache *cache = _xdg_mime_caches[i];

      xdg_uint32_t list_offset = GET_UINT32 (cache->buffer, 20);
      xdg_uint32_t n_entries = GET_UINT32 (cache->buffer, list_offset);
      xdg_uint32_t j;

      for (j = 0; j < n_entries && n < n_mime_types; j++)
	{
	  xdg_uint32_t offset = GET_UINT32 (cache->buffer, list_offset + 4 + 8 * j);
	  xdg_uint32_t mimetype_offset = GET_UINT32 (cache->buffer, list_offset + 4 + 8 * j + 4);
	  ptr = cache->buffer + offset;
	  mime_type = cache->buffer + mimetype_offset;

	  /* FIXME: Not UTF-8 safe */
	  if (fnmatch (ptr, file_name, 0) == 0)
	    mime_types[n++] = mime_type;
	}

      if (n > 0)
	return n;
    }

  return 0;
}

static int
cache_glob_node_lookup_suffix (XdgMimeCache *cache,
			       xdg_uint32_t  n_entries,
			       xdg_uint32_t  offset,
			       const char   *suffix,
			       int           ignore_case,
			       const char   *mime_types[],
			       int           n_mime_types)
{
  xdg_unichar_t character;
  xdg_unichar_t match_char;
  xdg_uint32_t mimetype_offset;
  xdg_uint32_t n_children;
  xdg_uint32_t child_offset;

  int min, max, mid, n;

  character = _xdg_utf8_to_ucs4 (suffix);
  if (ignore_case)
    character = _xdg_ucs4_to_lower (character);

  min = 0;
  max = n_entries - 1;
  while (max >= min)
    {
      mid = (min + max) /  2;

      match_char = GET_UINT32 (cache->buffer, offset + 16 * mid);

      if (match_char < character)
	min = mid + 1;
      else if (match_char > character)
	max = mid - 1;
      else
	{
	  suffix = _xdg_utf8_next_char (suffix);
	  if (*suffix == '\0')
	    {
              xdg_uint32_t i;
	      mimetype_offset = GET_UINT32 (cache->buffer, offset + 16 * mid + 4);
	      n = 0;
              if (cache->buffer[mimetype_offset])
                mime_types[n++] = cache->buffer + mimetype_offset;

	      n_children = GET_UINT32 (cache->buffer, offset + 16 * mid + 8);
	      child_offset = GET_UINT32 (cache->buffer, offset + 16 * mid + 12);
	      i = 0;
	      while (n < n_mime_types && i < n_children)
		{
		  match_char = GET_UINT32 (cache->buffer, child_offset + 16 * i);
		  mimetype_offset = GET_UINT32 (cache->buffer, offset + 16 * i + 4);
		  if (match_char != 0)
		    break;

		  mime_types[n++] = cache->buffer + mimetype_offset;
		  i++;
		}

	      return n;
	    }
	  else
	    {
	      n_children = GET_UINT32 (cache->buffer, offset + 16 * mid + 8);
	      child_offset = GET_UINT32 (cache->buffer, offset + 16 * mid + 12);

	      return cache_glob_node_lookup_suffix (cache,
						    n_children, child_offset,
						    suffix, ignore_case,
						    mime_types,
						    n_mime_types);
	    }
	}
    }

  return 0;
}

static int
cache_glob_lookup_suffix (const char *suffix,
			  int         ignore_case,
			  const char *mime_types[],
			  int         n_mime_types)
{
  int i, n;

  for (i = 0; _xdg_mime_caches[i]; i++)
    {
      XdgMimeCache *cache = _xdg_mime_caches[i];

      xdg_uint32_t list_offset = GET_UINT32 (cache->buffer, 16);
      xdg_uint32_t n_entries = GET_UINT32 (cache->buffer, list_offset);
      xdg_uint32_t offset = GET_UINT32 (cache->buffer, list_offset + 4);

      n = cache_glob_node_lookup_suffix (cache,
					 n_entries, offset,
					 suffix, ignore_case,
					 mime_types,
					 n_mime_types);
      if (n > 0)
	return n;
    }

  return 0;
}

static void
find_stopchars (char *stopchars)
{
  int i, k, l;

  k = 0;
  for (i = 0; _xdg_mime_caches[i]; i++)
    {
      XdgMimeCache *cache = _xdg_mime_caches[i];

      xdg_uint32_t list_offset = GET_UINT32 (cache->buffer, 16);
      xdg_uint32_t n_entries = GET_UINT32 (cache->buffer, list_offset);
      xdg_uint32_t offset = GET_UINT32 (cache->buffer, list_offset + 4);
      xdg_uint32_t j;

      for (j = 0; j < n_entries; j++)
	{
	  xdg_uint32_t match_char = GET_UINT32 (cache->buffer, offset);

	  if (match_char < 128)
	    {
	      for (l = 0; l < k; l++)
		if (stopchars[l] == (char)match_char)
		  break;
	      if (l == k)
		{
		  stopchars[k] = (char) match_char;
		  k++;
		}
	    }

	  offset += 16;
	}
    }

  stopchars[k] = '\0';
}

static int
cache_glob_lookup_file_name (const char *file_name,
			     const char *mime_types[],
			     int         n_mime_types)
{
  const char *ptr;
  char stopchars[128];
  int n;

  assert (file_name != NULL);

  /* First, check the literals */
  n = cache_glob_lookup_literal (file_name, mime_types, n_mime_types);
  if (n > 0)
    return n;

  find_stopchars (stopchars);

  /* Next, check suffixes */
  ptr = strpbrk (file_name, stopchars);
  while (ptr)
    {
      n = cache_glob_lookup_suffix (ptr, FALSE, mime_types, n_mime_types);
      if (n > 0)
	return n;

      n = cache_glob_lookup_suffix (ptr, TRUE, mime_types, n_mime_types);
      if (n > 0)
	return n;

      ptr = strpbrk (ptr + 1, stopchars);
    }

  /* Last, try fnmatch */
  return cache_glob_lookup_fnmatch (file_name, mime_types, n_mime_types);
}

int
_xdg_mime_cache_get_max_buffer_extents (void)
{
  xdg_uint32_t offset;
  xdg_uint32_t max_extent;
  int i;

  max_extent = 0;
  for (i = 0; _xdg_mime_caches[i]; i++)
    {
      XdgMimeCache *cache = _xdg_mime_caches[i];

      offset = GET_UINT32 (cache->buffer, 24);
      max_extent = MAX (max_extent, GET_UINT32 (cache->buffer, offset + 4));
    }

  return max_extent;
}

static const char *
cache_get_mime_type_for_data (const void *data,
			      size_t      len,
			      const char *mime_types[],
			      int         n_mime_types)
{
  const char *mime_type;
  int i, n, priority;

  priority = 0;
  mime_type = NULL;
  for (i = 0; _xdg_mime_caches[i]; i++)
    {
      XdgMimeCache *cache = _xdg_mime_caches[i];

      int prio;
      const char *match;

      match = cache_magic_lookup_data (cache, data, len, &prio,
				       mime_types, n_mime_types);
      if (prio > priority)
	{
	  priority = prio;
	  mime_type = match;
	}
    }

  if (priority > 0)
    return mime_type;

  for (n = 0; n < n_mime_types; n++)
    {
      if (mime_types[n])
	return mime_types[n];
    }

  return XDG_MIME_TYPE_UNKNOWN;
}

const char *
_xdg_mime_cache_get_mime_type_for_data (const void *data,
					size_t      len)
{
  return cache_get_mime_type_for_data (data, len, NULL, 0);
}

const char *
_xdg_mime_cache_get_mime_type_for_file (const char  *file_name,
					struct stat *statbuf)
{
  const char *mime_type;
  const char *mime_types[2];
  FILE *file;
  unsigned char *data;
  int max_extent;
  int bytes_read;
  struct stat buf;
  const char *base_name;
  int n;

  if (file_name == NULL)
    return NULL;

  if (! _xdg_utf8_validate (file_name))
    return NULL;

  base_name = _xdg_get_base_name (file_name);
  n = cache_glob_lookup_file_name (base_name, mime_types, 2);

  if (n == 1)
    return mime_types[0];

  if (!statbuf)
    {
      if (stat (file_name, &buf) != 0)
	return XDG_MIME_TYPE_UNKNOWN;

      statbuf = &buf;
    }

  if (!S_ISREG (statbuf->st_mode))
    return XDG_MIME_TYPE_UNKNOWN;

  /* FIXME: Need to make sure that max_extent isn't totally broken.  This could
   * be large and need getting from a stream instead of just reading it all
   * in. */
  max_extent = _xdg_mime_cache_get_max_buffer_extents ();
  data = malloc (max_extent);
  if (data == NULL)
    return XDG_MIME_TYPE_UNKNOWN;

  file = fopen (file_name, "r");
  if (file == NULL)
    {
      free (data);
      return XDG_MIME_TYPE_UNKNOWN;
    }

  bytes_read = fread (data, 1, max_extent, file);
  if (ferror (file))
    {
      free (data);
      fclose (file);
      return XDG_MIME_TYPE_UNKNOWN;
    }

  mime_type = cache_get_mime_type_for_data (data, bytes_read,
					    mime_types, n);

  free (data);
  fclose (file);

  return mime_type;
}

const char *
_xdg_mime_cache_get_mime_type_from_file_name (const char *file_name)
{
  const char *mime_types[2];

  if (cache_glob_lookup_file_name (file_name, mime_types, 2) == 1)
    return mime_types[0];
  else
    return XDG_MIME_TYPE_UNKNOWN;
}

#if 1
static int
is_super_type (const char *mime)
{
  int length;
  const char *type;

  length = strlen (mime);
  type = &(mime[length - 2]);

  if (strcmp (type, "/*") == 0)
    return 1;

  return 0;
}
#endif

int
_xdg_mime_cache_mime_type_subclass (const char *mime,
				    const char *base)
{
  const char *umime, *ubase;

  int i, min, max, med, cmp;

  umime = _xdg_mime_cache_unalias_mime_type (mime);
  ubase = _xdg_mime_cache_unalias_mime_type (base);

  if (strcmp (umime, ubase) == 0)
    return 1;

  /* We really want to handle text/ * in GtkFileFilter, so we just
   * turn on the supertype matching
   */
#if 1
  /* Handle supertypes */
  if (is_super_type (ubase) &&
      xdg_mime_media_type_equal (umime, ubase))
    return 1;
#endif

  /*  Handle special cases text/plain and application/octet-stream */
  if (strcmp (ubase, "text/plain") == 0 &&
      strncmp (umime, "text/", 5) == 0)
    return 1;

  if (strcmp (ubase, "application/octet-stream") == 0)
    return 1;

  for (i = 0; _xdg_mime_caches[i]; i++)
    {
      XdgMimeCache *cache = _xdg_mime_caches[i];

      xdg_uint32_t list_offset = GET_UINT32 (cache->buffer, 8);
      xdg_uint32_t n_entries = GET_UINT32 (cache->buffer, list_offset);
      xdg_uint32_t offset, n_parents, parent_offset;

      min = 0;
      max = n_entries - 1;
      while (max >= min)
	{
	  med = (min + max)/2;

	  offset = GET_UINT32 (cache->buffer, list_offset + 4 + 8 * med);
	  cmp = strcmp (cache->buffer + offset, umime);
	  if (cmp < 0)
	    min = med + 1;
	  else if (cmp > 0)
	    max = med - 1;
	  else
	    {
              xdg_uint32_t j;
	      offset = GET_UINT32 (cache->buffer, list_offset + 4 + 8 * med + 4);
	      n_parents = GET_UINT32 (cache->buffer, offset);

	      for (j = 0; j < n_parents; j++)
		{
		  parent_offset = GET_UINT32 (cache->buffer, offset + 4 + 4 * j);
		  if (_xdg_mime_cache_mime_type_subclass (cache->buffer + parent_offset, ubase))
		    return 1;
		}

	      break;
	    }
	}
    }

  return 0;
}

const char *
_xdg_mime_cache_unalias_mime_type (const char *mime)
{
  const char *lookup;

  lookup = cache_alias_lookup (mime);

  if (lookup)
    return lookup;

  return mime;
}

char **
_xdg_mime_cache_list_mime_parents (const char *mime)
{
  int i, p;
  char *all_parents[128]; /* we'll stop at 128 */
  char **result;

  mime = xdg_mime_unalias_mime_type (mime);

  p = 0;
  for (i = 0; _xdg_mime_caches[i]; i++)
    {
      XdgMimeCache *cache = _xdg_mime_caches[i];

      xdg_uint32_t list_offset = GET_UINT32 (cache->buffer, 8);
      xdg_uint32_t n_entries = GET_UINT32 (cache->buffer, list_offset);
      xdg_uint32_t j;

      for (j = 0; j < n_entries; j++)
	{
	  xdg_uint32_t mimetype_offset = GET_UINT32 (cache->buffer, list_offset + 4 + 8 * j);
	  xdg_uint32_t parents_offset = GET_UINT32 (cache->buffer, list_offset + 4 + 8 * j + 4);

	  if (strcmp (cache->buffer + mimetype_offset, mime) == 0)
	    {
	      xdg_uint32_t k;
	      xdg_uint32_t parent_mime_offset;
	      xdg_uint32_t n_parents = GET_UINT32 (cache->buffer, parents_offset);

	      for (k = 0; k < n_parents && p < 127; k++)
		{
		  parent_mime_offset = GET_UINT32 (cache->buffer, parents_offset + 4 + 4 * k);
		  all_parents[p++] = cache->buffer + parent_mime_offset;
		}

	      break;
	    }
	}
    }
  all_parents[p++] = 0;

  result = (char **) malloc (p * sizeof (char *));
  memcpy (result, all_parents, p * sizeof (char *));

  return result;
}
