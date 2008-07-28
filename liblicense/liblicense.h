/* Creative Commons has made the contents of this file
 * available under a CC-GNU-LGPL license:
 *
 * http://creativecommons.org/licenses/LGPL/2.1/
 *
 * A copy of the full license can be found as part of this
 * distribution in the file COPYING.
 *
 * You may use the liblicense software in accordance with the
 * terms of that license.  You agree that you are solely
 * responsible for your use of the liblicense software and you
 * represent and warrant to Creative Commons that your use
 * of the liblicense software will comply with the CC-GNU-LGPL.
 *
 * Copyright 2007, Creative Commons, www.creativecommons.org.
 * Copyright 2007, Scott Shawcroft.
 * Copyright (C) 2007 Peter Miller
 */

#ifndef LIBLICENSE_H
#define LIBLICENSE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @mainpage
 *
 * A library for managing license metadata, in particular Creative
 * Commons (CC) licensing information.
 *
 * The idea of this library started out on the IRC channel, in a
 * discussion on how to best help boost the community of software
 * developers working with things related to CC - mostly the metadata
 * format.
 *
 * Basically, the idea is to write a portable C library that manages
 * metadata for CC licenses, and a bunch of other licenses of interest
 * to the community.
 *
 * The library will produce licensing information based on the
 * specifications of calling libraries and programs.  In addition
 * to generating text for specific licenses, it will also allow an
 * application to enumerate which licenses are currently available
 * and provide descriptive text for each license, and for license
 * features.  It should also provide an easy way to specify "verify at"
 * URLs.
 *
 * The benefit of this library is that applications linking to it
 * can correctly offer licensing choices, and these choices can
 * be transparently updated through package managers as license
 * versions are updated.  Human readable descriptions will also be
 * internationalized, preferably using the same .po files used by the CC
 * web site.  Hence liblicense will take advantage of package updating
 * and i18n systems to allow applications to always provide current
 * and correct licensing choices and license text.
 *
 * Part of the project is also to provide wrappers for the library for
 * other languages, and to help external developers add metadata support
 * to their projects.  A good start will probably be to wrap the library
 * for Python, and use it for ccpublisher.
 *
 * We will also integrate a module system so that libraries can be used
 * to embed and extract metadata in/from common formats.
 *
 * As liblicense itself deals only with text strings, we can also make
 * GUI libraries to provide dialogs which present these strings to the
 * user in desktop or web applications.  This layering ensures that such
 * dialogs present consistent licensing choices.
 *
 * @section N01 Getting Started
 *
 * Before you can call the library, it is mandatory to call the #ll_init
 * function.  This is required to initialize internal databases and
 * indexes.
 *
 * Once you are done with the library, it is important to call the
 * #ll_stop function.  This ensures that library resources are free()ed,
 * but more importantly it ensures that caches that need to be have been
 * flushed.
 *
 *
 * @section N02 Discovering Available Licenses
 *
 * To obtain a list of available licenses, you use the #ll_get_licenses
 * function.  This returns a list of license URIs.  This is the most
 * important thing to understand about liblicense: it uses
 * URIs to name everything, including license references.
 *
 * For example, the Sampling Plus license URI looks like this:
 * http://creativecommons.org/licenses/sampling+/1.0/
 *
 * The <i>license -a</i> command uses code very similar to the following
 * to list all of the available licenses:
 *
 * \code
 *   #include <liblicense.h>
 *
 *   int
 *   main (int argc, char **argv)
 *   {
 *     ll_uri_t *licenses;
 *     int i;
 *
 *     licenses = ll_get_licenses (NULL);
 *     i = 0;
 *     while (licenses[i] != NULL)
 *       {
 *         printf ("%s - %s\n", ll_get_first(ll_get_attribute(licenses[i], LL_NAME, false)), licenses[i]);
 *         i++;
 *       }
 *     ll_free_list (licenses);
 *     ll_stop ();
 *     return 0;
 *   }
 * \endcode
 *
 * The LL_NAME, attribute used to obtain a human-friendly license
 * name.  You <b>can not</b> pass a human-friendly name to any of the
 * functions which expect a license URI, they simply will not work.  You
 * will have to use #ll_get_licenses and walk the list to translate a
 * human-friendly name into a license URI.
 *
 * There is some logic behind all of this: the human-fiendly names are
 * internationalized, and will be appropriately translated for the
 * current locale.  This means the human-fiendly names are not constant
 * from one country to the next, but the license URIs are the same the
 * world over.
 *
 *
 * @section N03 Getting a License from a File
 *
 * There is one thing you will want to do over and over again:
 * read the license of a file.  Once you have the license URI, you can
 * query the license for the various rights you have.
 * First things first: reading the license.
 *
 * \code
 *   #include <liblicense.h>
 *   #include <stdio.h>
 *
 *   int
 *   main (int argc, char **argv)
 *   {
 *     ll_uri_t uri;
 *
 *     ll_init ();
 *     uri = ll_read (argv[1], LL_LICENSE);
 *     if (uri == NULL)
 *       printf ("No license found\n");
 *     else
 *       printf ("%s\n", uri);
 *     ll_stop ();
 *     return 0;
 *   }
 * \endcode
 *
 * Some file formats are capable of holding a license within their other
 * application data.  For example, PNG files have optional text comment
 * blocks, and the license data could be stored in them.  For formats
 * which cannot carry license data, is is also possible to a have a
 * companion ".xmp" license file.  The ll_read function checks for both
 * possibilities.
 *
 *
 * @section N04 Getting the Attributes of a License
 *
 * Having read the license of a file, you want to know what conditions
 * it imposes.  There are several attributes liblicense exposes for this purpose.
 * We have already seen the LL_NAME attribute.  The others to
 * use are LL_VERSION, #ll_get_jurisdiction, LL_PERMITS,
 * LL_REQUIRES, and LL_PROHIBITS.
 *
 * \code
 *   void
 *   print_license_info (ll_uri_t uri)
 *   {
 *     char **attrs, **attr;
 *     char *string;
 *     ll_version_t version;
 *     ll_juris_t juris;
 *     int i;
 *
 *     printf ("License URI: %s\n", uri);
 *
 *     string = ll_get_first(ll_get_attribute(uri, LL_NAME, false));
 *     printf ("Name: %s\n", string);
 *     free (string);
 *
 *     version = ll_get_version (uri);
 *     printf ("Version: ");
 *     if (version)
 *       {
 *         for (i = 1; i <= version[0]; ++i)
 *           {
 *             if (i != 1)
 *               printf (".");
 *             printf ("%d", version[i]);
 *           }
 *         printf ("\n");
 *         free (version);
 *       }
 *
 *     juris = ll_get_first(ll_get_attribute(uri, LL_JURISDICTION, false));
 *     if (juris)
 *       {
 *         string = ll_jurisdiction_name (juris);
 *         printf ("Jurisdiction: %s (%s)\n", string, juris);
 *         free (string);
 *         free (juris);
 *       }
 *
 *     printf ("Rights:\n");
 *
 *     attrs = ll_get_attribute(uri, LL_PERMITS, false);
 *     if (*attrs)
 *       {
 *         printf ("   Permits:\n");
 *         for (attr = attrs; *attr; ++attr)
 *           {
 *             printf ("      %s\n", *attr);
 *           }
 *       }
 *     ll_free_list (attrs);
 *
 *     attrs = ll_get_attribute(uri, LL_REQUIRES, false);
 *     if (*attrs)
 *       {
 *         printf ("   Requires:\n");
 *         for (attr = attrs; *attr; ++attr)
 *           {
 *             printf ("      %s\n", *attr);
 *           }
 *       }
 *     ll_free_list (attrs);
 *
 *     attrs = ll_get_attribute(uri, LL_PROHIBITS, false);
 *     if (*attrs)
 *       {
 *         printf ("   Prohibits:\n");
 *         for (attr = attrs; *attr; ++attr)
 *           {
 *             printf ("      %s\n", *attr);
 *           }
 *       }
 *     ll_free_list (attrs);
 *   }
 * \endcode
 *
 * When you actually compile and run this code, you will see that once
 * again there are URIs for each of the rights, rather than simple
 * strings.
 *
 * The above code is a shortened version of the #ll_license_print_info
 * function, which you may prefer to use for consistency.
 *
 * \code
 *   #include <liblicense.h>
 *   #include <stdio.h>
 *
 *   int
 *   main (int argc, char **argv)
 *   {
 *     ll_uri_t uri;
 *
 *     ll_init ();
 *     uri = ll_read (argv[1], LL_LICENSE);
 *     if (uri == 0)
 *       printf ("No license found\n");
 *     else
 *       ll_license_print_info (uri);
 *     ll_stop ();
 *     return 0;
 * \endcode
 *
 * And, as you can see, the amount of code you need to write is much
 * smaller, as well.
 *
 *
 * @section N05 Examining other properties of a file
 *
 * Despite its name, liblicense supports reading and writing other
 * properties to a file.  For example, you may want to store a link to a
 * "web statement" that has more information on the file.
 *
 * We have already seen the LL_LICENSE property you can pass to have ll_read().
 * The other supported properties are:
 * - LL_WEBSTATEMENT - see http://wiki.creativecommons.org/WebStatement
 * - LL_MORE_PERMISSIONS - see http://wiki.creativecommons.org/CCPlus
 *
 * To use them, just pass them to ll_read() like you did with LL_LICENSE.
 * Note that ll_write() also accepts these arguments in a similar way.
 *
 * @section N99 Discovering Supported Formats
 *
 * The easiest way to see a list of supported formats is to use the
 * <b>-m</b> option of the <i>license</i>(1) command which is part of
 * the liblicense package.  This program uses the liblicense API to
 * obtain this information as follows:
 *
 * \code
 *   #include <liblicense.h>
 *
 *   int
 *   main (int argc, char **argv)
 *   {
 *     ll_init ();
 *     ll_print_module_info ();
 *     ll_stop ();
 *     return 0;
 *   }
 * \endcode
 *
 * The results are printed on the standard output.  (This section needs
 * to be expanded for folks who need pick lists for GUIs._
 */

#ifndef WIN32
#define LL_DIR_SEPARATOR '/'
#define LL_DIR_SEPARATOR_S "/"
#else
#define LL_DIR_SEPARATOR '\\'
#define LL_DIR_SEPARATOR_S "\\"
#endif

/******************* license_info *******************/
typedef char *ll_juris_t;
typedef char *ll_uri_t;
typedef char *ll_locale_t;
typedef int *ll_version_t;
typedef char *ll_attribute_t;
typedef char *ll_filename_t;

/**
 * The LL_WEBSTATEMENT attribute is the URL
 * to a web page making a license assertion about
 * a document.  See http://wiki.creativecommons.org/Embedded_Metadata
 * for a comic!
 */
#define LL_WEBSTATEMENT "http://ns.adobe.com/xap/1.0/rights/WebStatement"

  /**
   * The LL_LICENSE attribute is the URI of a license
   * that permits some sharing of this work.
   */
#define LL_LICENSE "http://creativecommons.org/ns#license"

  /**
   * The LL_MORE_PERMISSIONS attribute is the URI of a web page
   * indicating how to get more permissions than the embedded license.
   */
#define LL_MORE_PERMISSIONS "http://creativecommons.org/ns#morePermissions"

/**
 * The LL_ATTRIBUTION string may be returned by the
 * LL_PERMITS, LL_REQUIRES and LL_PROHIBITS attributes.
 *
 * credit be given to copyright holder and/or author
 */
#define LL_ATTRIBUTION "http://creativecommons.org/ns#Attribution"

/**
 * The LL_COMMERCIAL_USE string may be returned by the
 * LL_PERMITS, LL_REQUIRES and LL_PROHIBITS attributes.
 *
 * exercising rights for commercial purposes
 */
#define LL_COMMERCIAL_USE "http://creativecommons.org/ns#CommercialUse"

/**
 * The LL_DERIVATIVE_WORKS string may be returned by the
 * LL_PERMITS, LL_REQUIRES and LL_PROHIBITS attributes.
 *
 * creation and distribution of derivative works
 */
#define LL_DERIVATIVE_WORKS "http://creativecommons.org/ns#DerivativeWorks"

/**
 * The LL_DISTRIBUTION string may be returned by the
 * LL_PERMITS, LL_REQUIRES and LL_PROHIBITS attributes.
 *
 * distribution, public display, and publicly performance
 */
#define LL_DISTRIBUTION "http://creativecommons.org/ns#Distribution"

/**
 * The LL_HIGH_INCOME_NATION_USE string may be returned
 * by the LL_PERMITS, LL_REQUIRES and LL_PROHIBITS
 * functions.
 *
 * use in a non-developing country
 */
#define LL_HIGH_INCOME_NATION_USE "http://creativecommons.org/ns#HighIncomeNationUse"

/**
 * The LL_NOTICE string may be returned by the
 * LL_PERMITS, LL_REQUIRES and LL_PROHIBITS attributes.
 *
 * copyright and license notices be kept intact
 */
#define LL_NOTICE "http://creativecommons.org/ns#Notice"

/**
 * The LL_REPRODUCTION string may be returned by the
 * LL_PERMITS, LL_REQUIRES and LL_PROHIBITS attributes.
 *
 * making multiple copies
 */
#define LL_REPRODUCTION "http://creativecommons.org/ns#Reproduction"

/**
 * The LL_SHARE_ALIKE string may be returned by the
 * LL_PERMITS, LL_REQUIRES and LL_PROHIBITS attributes.
 *
 * derivative works be licensed under the same terms as the original work
 */
#define LL_SHARE_ALIKE "http://creativecommons.org/ns#ShareAlike"

/**
 * The LL_SHARING string may be returned by the
 * LL_PERMITS, LL_REQUIRES and LL_PROHIBITS attributes.
 *
 * noncommercial distribution, permits commercial derivatives, but only
 * non-commercial distribution
 */
#define LL_SHARING "http://creativecommons.org/ns#Sharing"

/**
 * The LL_SOURCE_CODE string may be returned by the
 * LL_PERMITS, LL_REQUIRES and LL_PROHIBITS attributes.
 *
 * source code (the preferred form for making modifications) must be
 * provided for all derivative works
 */
#define LL_SOURCE_CODE "http://creativecommons.org/ns#SourceCode"

/**
 * The LL_REPLACED_BY string can be passed to the
 * #ll_get_attribute function to obtain a license's successor.
 */
#define LL_REPLACED_BY "http://purl.org/dc/terms/isReplacedBy"

/**
 * The LL_JURISDICTION string can be passed to the
 * #ll_get_attribute function to obtain a license's jurisdiction.  The
 * #ll_get_jurisdiction function does this very thing.
 */
#define LL_JURISDICTION "http://creativecommons.org/ns#jurisdiction"

/**
 * The LL_NAME string can be passed to the
 * #ll_get_attribute function to obtain a license's name.
 * Example: "Creative Commons Attribution 3.0 Unported"
 */
#define LL_NAME "http://purl.org/dc/elements/1.1/title"

/**
 * The LL_VERSION string can be passed to the
 * #ll_get_attribute function to obtain a license's version.  The
 * LL_VERSION function does this very thing.
 */
#define LL_VERSION "http://purl.org/dc/terms/hasVersion"

/**
 * The ll_parse_version function is used to parse a version string
 * into a heap-allocated array of integers.
 * 
 * Typically this follows a call to ll_get_attribute() requesting
 * the #LL_VERSION attribute.
 *
 * @param version
 *     The version number as a string
 * @returns
 *     an array of integers representing each division.  The first
 *     element of the array specifies the number of version divisions
 *     that follow, and the rest are the actual version numbers.
 *     Use free() when you are done with it.
 */
ll_version_t ll_parse_version (const char * version);

/**
 * The LL_CREATOR string can be passed to the
 * #ll_get_attribute function to obtain a license's creator.
 *
 * Example: "Creative Commons"
 */
#define LL_CREATOR "http://purl.org/dc/elements/1.1/creator"

/**
 * The LL_PROHIBITS string can be passed to the
 * #ll_get_attribute function to obtain a list of the rights prohibited
 * by a license.  The LL_PROHIBITS function does this very
 * thing.
 */
#define LL_PROHIBITS "http://creativecommons.org/ns#prohibits"

/**
 * The LL_PERMITS string can be passed to the
 * #ll_get_attribute function to obtain a list of the rights permitted
 * by a license.  The LL_PERMITS function does this very
 * thing.
 */
#define LL_PERMITS "http://creativecommons.org/ns#permits"

/**
 * The LL_REQUIRES string can be passed to the
 * #ll_get_attribute function to obtain a list of the rights required
 * by a license.  The LL_REQUIRES function does this very
 * thing.
 */
#define LL_REQUIRES "http://creativecommons.org/ns#requires"

/**
 * The ll_get_attribute function is used to obtain the names of all
 * attributes of the given license which have the given URI.
 *
 * @param license_uri
 *     The license of interest
 * @param attribute_uri
 *     The attribute URI of interest (e.g. #LL_PERMITS)
 * @param locale
 *     non-zero if $LANG to be used,
 *     zero if not relevant
 * @returns
 *     a NULL terminated list of strings,
 *     use #ll_free_list when you are done with it.
 */
char **ll_get_attribute (ll_uri_t license_uri, const char *attribute_uri,
                         int locale);

/**
 * The ll_filename_to_uri function is used to convert a filename string
 * to a URI.  The last four characters (the 3 character extension and
 * the dot, assuming ".xmp") are ignored.  Underscore characters (_)
 * are mapped to slash (/).  It is happed to a http:// style URI, it is
 * <b>not</b> mapped into a file:/// style URI.
 *
 * Given a license RDF file found on disk, this is used to repopulate
 * what URI the RDF refers to.
 *
 * @param filename
 *     The file name to be converted to a URI.
 * @returns
 *     a new string.  use free() when you are done with it.
 */
ll_uri_t ll_filename_to_uri (const ll_filename_t filename);

/**
 * The ll_uri_to_filename function is used to convert a URI string to
 * a filename.  The http:// prefix is removed, and a ".rdf" extension
 * is added.  The slash (/) characters are mapped to underscore (_)
 * characters.
 *
 * Given a license URI we need to store or read on disk, this is used
 * to determine what filename to save the RDF as or where to read it
 * from.
 *
 * @param uri
 *     The uri to be converted
 * @returns
 *     a new string.  Use free() when you are done with it.
 *
 * @note
 *     this function and the #ll_filename_to_uri function are not
 *     completely symmetric.  Don't expect
 *         strcmp(ll_uri_to_filename(ll_filename_to_uri(x)), x) == 0
 *     and do not expect
 *         strcmp(ll_filename_to_uri(ll_uri_to_filename(x)), x) == 0
 */
ll_filename_t ll_uri_to_filename (const ll_uri_t uri);


  /**
   * Simple function to get the first element of a list
   * FIXME: Using this directly may lead to memory leaks!
   * @param list
   * @returns The first element of the list passed in, a char *.
   */
  char * ll_get_first(char** list);

/**
 * The ll_jurisdiction_name functions is used to map a jusrisdiction
 * code (two letter country code, usually) to a jusrisdiction name
 * (country name).
 *
 * @param jurisdiction
 *     The justisdiction code of interest
 * @returns
 *     a string containing the jurisdiction name (country name),
 *     or NULL on error.  Use free when you are done with it.
 */
char *ll_jurisdiction_name (const char * jurisdiction);

/*******************************************************/

/************** licenses_{cached,uncached} *************/

/**
 * The ll_init function is used to prepare the liblicense library for
 * use.  This function is to be called before any other use of the
 * library.
 *
 * @returns
 *     0 on success,
 *     1 if there was a problem (printed on stderr)
 */
int ll_init (void);

/**
 * The ll_stop function is used to release all resources held by
 * the library.  It must be used after all uses of the library are
 * completed... there may be writes and updates to perform before the
 * program completes.
 *
 * @returns
 *     0 on success
 */
int ll_stop (void);

/**
 * The ll_verify_uri function is used to determine whether or not the
 * given uri is recognized by the system.
 *
 * @param license_uri
 *     The URI of the license of interest
 * @returns
 *     1 if recognized,
 *     0 if not
 */
int ll_verify_uri (const ll_uri_t license_uri);

/**
 * The ll_get_all_licenses function is used to obtain a list of all
 * general license URIs available for new works.
 *
 * @returns
 *     a NULL terminated list of strings.
 *     Use #ll_free_list when you are done with it.
 */
ll_uri_t *ll_get_all_licenses (void);

/**
 * The ll_get_licenses function is used to obtain a list of all general
 * licenses in a family.
 *
 * @param justrisdiction
 *     A string naming the jurisdiction code of interest.
 * @returns
 *     a NULL terminated list of strings containing URIs of licenses.
 *     Use #ll_free_list when you are done with it.
 */
ll_uri_t *ll_get_licenses (const ll_juris_t justrisdiction);

/**
 * The ll_get_jurisdictions function is used to obtain a list of all
 * jurisdiction codes.
 *
 * @returns
 *     a NULL terminated list of strings containing available
 *     juristriction codes (as opposed to all possible jurisdiction
 *     codes).  Use #ll_free_list when you are done with it.
 */
ll_juris_t *ll_get_jurisdictions (void);

/**
 * The ll_license_print_info function is used to print all available
 * details of a license on the standard output.
 *
 * @param license_uri
 *     The URI of the license who's details are to be printed.
 */
void ll_license_print_info (ll_uri_t license_uri);

/******************* module_wrangler *******************/

  /**
   * LL_PREDICATE_ANY is a string meaning that a module can store
   * any URI as a predicate.
   */
#define LL_PREDICATE_ANY "?predicate"

typedef char *ll_module_t;
typedef char *ll_symbol_t;
typedef char *ll_mime_type_t;

/**
 * The ll_get_config_modules function may be used to obtain a list of
 * available configuration modules.
 *
 * @returns
 *     a NULL terminated list of config module filenames (strings).
 *     Use #ll_free_list when you are done with it.
 */
ll_module_t *ll_get_config_modules (void);

/**
 * The ll_get_io_modules function may be used to obtain a list of
 * available I/O modules.
 *
 * @returns
 *     a NULL terminated list of I/O module filenames (strings).
 *     Use #ll_free_list when you are done with it.
 * @note
 *     The #ll_init_modules function shall be called before this function.
 */
ll_module_t *ll_get_io_modules (void);

/**
 * The ll_module_init function may be used to load a module and call
 * its "init" function with no arguments.  If the module does not have an
 * "init" symbol, nothing is called, but the module is still dlopen()ed.
 * No checks are made that the "init" symbol is a functions (and not,
 * say, a variable) and no checks are made that the function takes no
 * arguments.
 *
 * Each successfull call <i>must</i> be balanced by a #ll_module_shutdown
 * call when you are done with the module.
 *
 * @param directory
 *     The directory containing the modules.
 * @param module
 *     The name of the module of interest
 * @returns
 *     0 if the module can't be opened with dlopen,
 *     1 if it can.
 */
int ll_module_init (const char *directory, ll_module_t module);

/**
 * The ll_get_module_symbol function may be used to obtain a pointer to
 * a symbol in a module.  Usually this will be a function, but it could
 * be data.
 *
 * @param directory
 *     The directory containing the modules.
 * @param module
 *     The name of the module of interest
 * @param symbol
 *     The name of the symbol of interest
 * @returns
 *     the NULL pointer if the symbol does not exist, or a pointer to
 *     the symbol in dynamic memory.
 * @note
 *     This function will exit with a fatal error if it is not possble
 *     to dlopen the module.
 */
void *ll_get_module_symbol (const char *directory, ll_module_t module,
                            ll_symbol_t symbol);

/**
 * The ll_module_shutdown function may be used to call the shutdown
 * symbol (if it exists) of the module and then unloads the module.  No
 * checks are made that the "shutdown" symbol is a function (and not,
 * say, a variable) and no checks are made that the function takes no
 * arguments.
 *
 * @param directory
 *     The directory containing the modules.
 * @param module
 *     The name of the module of interest
 * @returns
 *     -1 on error (the module cannot be dlopen()ed),
 *     0 on success
 */
int ll_module_shutdown (const char *directory, ll_module_t module);

/**
 * The ll_print_module_info function is used to print a human readable
 * summary of all available I/O modules on the standard output.
 *
 * @note
 *     The #ll_init_modules function shall be called before this function.
 */
void ll_print_module_info (void);

/**
 * The ll_init_modules function is used to initialize the I/O module
 * system before any of the module handling functions are called.
 *
 * It is safe to call this function more than once, but this does <b>not</b>
 * increase a reference count, so the first call to #ll_stop_modules will
 * stop all modules.
 *
 * @note
 *     If an error occurs reading the modules directory, an error will
 *     be reported on stderr, and this function will return without
 *     doing anything.
 * @note
 *     Clients of this API should never need to call this function
 *     directly, as the #ll_init functions calls it for you.
 */
void ll_init_modules (void);

/**
 * The ll_stop_modules function is used to finalize the I/O module
 * system after the last use of the module system has completed.
 *
 * It is mostly safe to call this function more than once.  See the note
 * on #ll_init_modules for more information.
 *
 * @note
 *     Clients of this API should never need to call this function
 *     directly, as the #ll_stop functions calls it for you.
 */
void ll_stop_modules (void);

/**
 * The _ll_contains_token function is used to look for a given token in
 * a string.
 *
 * @param haystack
 *     The string to be searched
 * @param needle
 *     The string to be searched for
 * @returns
 *     non-zero (true) if the needle is found in the haystack,
 *     zero (false) if not found.
 */
int _ll_contains_token (const char **haystack, const char *needle);

/**
 * Calculates the number of modules available to us.
 * @return int # of modules available...
 */
unsigned int _ll_modules_count_available();

typedef struct _LLModuleDesc LLModuleDesc;

extern LLModuleDesc **_ll_module_list;

typedef void (*LLModuleInitFunc) (void);
typedef char *(*LLModuleReadFunc) (const char *, const char *);
typedef int (*LLModuleWriteFunc) (const char *, const char *, const char *);
typedef void (*LLModuleShutdownFunc) (void);

enum ll_features
{
  LL_FEATURES_NONE = 0x000000,
  LL_FEATURES_EMBED = 0x000001
};
typedef enum ll_features ll_features;

struct _LLModuleDesc
{
  const char *name;
  const char *description;
  const char *version;
  int features;
  const char **supported_predicates;

  const char **mime_types;
  LLModuleInitFunc module_init;
  LLModuleReadFunc read;
  LLModuleWriteFunc write;
  LLModuleShutdownFunc module_shutdown;
  void *handle;
};

#ifdef _MSC_VER
#define LL_MODULEEXPORT __declspec(dllexport) extern
#else
#define LL_MODULE_EXPORT
#endif

#define LL_MODULE_DEFINE(name,description,version,features,supported_predicates,mime_types,init,read,write,shutdown) \
LL_MODULE_EXPORT LLModuleDesc ll_module_desc = {        \
name,                                                   \
description,                                            \
version,                                                \
features,                                               \
supported_predicates,                                   \
mime_types,                                             \
init,                                                   \
read,                                                   \
write,                                                  \
shutdown ,                                              \
0                                                       \
};

/******************** system_default *******************/


/**
 * The ll_license_default_set function is used to set a specific license
 * as the default license.
 *
 * @param license_uri
 *     the URL of the license to be remembered as the default license
 * @returns
 *     1 on succes, or
 *     0 on failure
 */
int ll_license_default_set (const ll_uri_t license_uri);

/**
 * The ll_license_default_get function is used to obtain the URI of the
 * default license.
 *
 * @returns
 *     NULL if there is no default, or
 *     a pointer to a new string.  Use free() when you are done with it.
 */
ll_uri_t ll_license_default_get (void);

/********************* read_license ********************/

/**
 * The ll_read function may be used to obtain the URI of the license
 * of the given file.  All available modules will be tried against the
 * file.
 *
 * @param filename
 *     The name of the file for which the license is desired.
 * @param predicate 
 *         Read this predicate from the file.
 *         NULL is accepted for simplicity;
 *         it is equivalent to asking for the license of the file.
 *         Equivalently, you can ask for the LL_LICENSE predicate.
 * @returns
 *     NULL if the file cannot be read, or no license can be found; otherwise
 *     a string containing the URI of the license corresponding to the file.
 *     (sort of: it actually returns the license with the highest count)
 * @note
 *     The #ll_init_modules function shall be called before this function.
 */
  ll_uri_t ll_read (ll_filename_t filename, const ll_uri_t predicate);

/**
 * The ll_module_read function may be used to obtain the URI of the
 * license of the given file, as determined by the named module.
 *
 * @param filename
 *     The name of the file for which the license is desired
 * @param predicate 
 *         Read this predicate from the file.
 *         NULL is accepted for simplicity;
 *         it is equivalent to asking for the license of the file.
 *         Equivalently, you can ask for the LL_LICENSE predicate.
 * @param module
 *     The name of the module to scan the file with
 * @returns
 *     a string containing the URI of the license corresponding to the file.
 *     CHECKME: Probably could also return NULL, just like ll_read().
 * @note
 *     The #ll_init_modules function shall be called before this function.
 */
ll_uri_t ll_module_read (ll_filename_t filename, const ll_uri_t predicate, ll_module_t module);

typedef struct _LLModuleDesc_and_index {
	LLModuleDesc __desc;
	unsigned int __index;
	unsigned int __cached_module_list_length;
	char * __mime_type;
} LLModuleSearchState;

/**
 * Calculates the number of modules available to us.
 * @return int # of modules available...
 */
unsigned int ll_modules_count_available();

/**
 ** LLModuleSearchState state = {0};
 ** ll_module_for_file("/your/mom/music.mp3", LL_LICENSE, &state);
 ** modifies a LLModuleSearchState struct passed in
 ** so that the search can be resumed
 ** (without index, searching would be O(N^2)
 * @param filename
 *         The filename to search for. If NULL, then get every module.
 * @param predicate 
 *         Return only modules that support this predicate.  Use
 *         NULL if you want to find all modules independent
 *         of which predicate they support.  Using LL_PREDICATE_ANY
 *         will actually search for modules that literally support any
 *         predicate.
 * @return LLModuleDesc for the currently-found module
 *         If that is NULL, there are no more matching modules.
 */
  LLModuleDesc * ll_module_search(ll_filename_t filename, 
				  const char * predicate,
				  /* out */ LLModuleSearchState * state);

/******************** write_license ********************/

/**
 * The ll_write function is used to write the given license to the given
 * file using all applicable modules.
 *
 * It first attempts to embed the license into the file, it only writes
 * to external locations if it cannot embed the license using any
 * module.
 *
 * @param filename
 *     The name of the file the license is about
 * @param predicate
 *     The exact predicate being stored (NULL is okay if the predicate is license)
 * @param value
 *     The value to store (typically a URL/URI for a license)
 * @returns
 *     -1 if no embedders are available for this file type;
 *     0 if all available embedders failed;
 *     1 on success
 * @note
 *     The #ll_init_modules function shall be called before this function.
 */
  int ll_write (ll_filename_t filename, const ll_uri_t predicate, ll_uri_t value);

/**
 * The ll_module write function is used to write the given license to
 * the given file using the given module.
 *
 * @param filename
 *     The name of the file the license is about
 * @param predicate
 *     The exact predicate being stored (NULL is okay if the predicate is license)
 * @param value
 *     The value to store (typically a URL/URI for a license)
 * @param module
 *     The name of the module to use to write the license to the file.
 * @returns
 *     -1 if no embedders are available for this file type;
 *     0 if all available embedders failed;
 *     1 on success
 * @note
 *     The #ll_init_modules function shall be called before this function.
 */
  int ll_module_write (ll_filename_t filename, ll_uri_t predicate,
		       const ll_uri_t license_uri,
		       ll_module_t module);

/**
 * A constant indicating that module writing failed.
 */
#define LL_E_MODULE_WRITE_FAIL 1

/******************* license_chooser *******************/

/**
 * This is an opaque type, used to remember the results of a license query.
 */
typedef struct _ll_license_chooser_t ll_license_chooser_t;

typedef struct ll_license_list_t ll_license_list_t;
struct ll_license_list_t
{
  struct ll_license_list_t *next;
  const char *license;
};

#define LL_UNSPECIFIED 0

/**
 * The ll_new_license_chooser function is used to find a suitable
 * license given a list of license attributes.
 *
 * @param jurisdiction
 *     The two letter country code of the jurisdiction of interest
 * @param attributes
 *     a NULL terminated list of attribute names
 * @returns
 *     an opaque pointer to a result.
 *     Free with #ll_free_license_chooser when you are done with it.
 */
ll_license_chooser_t *ll_new_license_chooser (const ll_juris_t jurisdiction,
                                              const char **attributes);

/**
 * The ll_free_license_chooser function is used to release the resources
 * used by the result of a #ll_new_license_chooser function call.
 *
 * @param choo
 *     the license chooser results to be released
 */
void ll_free_license_chooser (ll_license_chooser_t *choo);

/**
 * The ll_get_licenses_from_flags function is used to extract a
 * linked-list of matching licenses from the result returned by a call
 * to #ll_new_license_chooser.  The flags given should be OR'ed together
 * and come from the result of ll_attribute_flag()
 *
 * @param choo
 *     The results from an earlier #ll_new_license_chooser call.
 * @param permits
 *     bit mask
 * @param requires
 *     bit mask
 * @param prohibits
 *     bit mask
 * @returns
 *     list of URIs of compatible licenses
 */
const ll_license_list_t *ll_get_licenses_from_flags (ll_license_chooser_t *choo,
                                                     int permits, int requires,
                                                     int prohibits);

  /** Given a license chooser object and a license,
   * this function determines what "permits", "requires", and "prohibits"
   * arguments to the chooser would be necessary to find the license.
   *
   * If the license cannot be found by the chooser, the flags are set to -1.
   * 
   * Only useful for debugging, as far as I know.
   * @param choo
   *       A chooser created by #ll_new_license_chooser.
   * @param license
   *       The license to explain how to find.
   * @param permits
   *       (modified) set to the permits value necessary to find this license
   * @param requires
   *       (modified) set to the requires value necessary to find this license
   * @param prohibits
   *       (modified) set to the prohibits value necessary to find this license
   *
   */
void ll_get_license_flags (ll_license_chooser_t *choo, const char *license,
                                                     int *permits, int *requires,
                                                     int *prohibits);


/**
 * The ll_attribute_flag function is used to obtain the bit map bit
 * corresponding to the named attribute in the #ll_free_license_chooser
 * result.  This is used to query the result for licenses.
 *
 * @param choo
 *     The results from an earlier #ll_new_license_chooser call.
 * @param attr
 *     The name of the attribute
 * @returns
 *     integer with single bit set
 */
int ll_attribute_flag (ll_license_chooser_t * choo, const char *attr);

/************************* list ************************/

/**
 * The ll_new_list function is used to allocate a list large enough to
 * hold the given number of strings.  It will be created empty.
 *
 * When you add strings to the list, make sure you use the strdup()
 * function, or any other method which directly or indirectly uses
 * malloc.  This is because the #ll_free_list function will pass it to
 * free().
 *
 * @param length
 *     the size of the new list (>= 0)
 * @returns
 *     a ponter to the new list.  Use #ll_free_list when you are done
 *     with it.
 */
char **ll_new_list (int length);

/**
 * The ll_free_list function is used to release memory used by a string
 * list created by the @ll_new_list function.
 *
 * @param list
 *     The list of strings to be free()ed.  Nothing bad happens if it is
 *     NULL.  Very bad things happen if you call it too many times for
 *     the same list.
 */
void ll_free_list (char **list);

/**
 * The ll_list_contains function is used to examine a list of strings to
 * determine whether or not it contains a specific string.
 *
 * @param haystack
 *     The list of strings to be searched
 * @param needle
 *     The string being searched for.
 * @returns
 *     false if haystack is NULL,
 *     false if needle is NULL,
 *     false if the needle is not present in the haystack,
 *     true only if the needle <b>is</b> present in the haystack,
 */
int ll_list_contains (char **haystack, char *needle);

/**
 * The ll_list_index function is used to determine the index of a string
 * in a list of strings.
 *
 * @param haystack
 *     The list of strings to be searched
 *     <b>Must not</b> be NULL.
 * @param needle
 *     The string being searched for.
 *     <b>Must not</b> be NULL.
 * @returns
 *     the index of the needle within the haystack, or
 *     -1 if not present at all
 */
int ll_list_index (char **haystack, char *needle);

/**
 * The ll_list_length function is used to
 * determine the length of a list of strings.
 *
 * @param list
 *     The list of strings to measure.
 *     <b>Must not</b> be NULL.
 * @returns
 *     The number of strings in the list.
 */
int ll_list_length (char **list);

/**
 * The ll_list_mode function is used to determine the string which
 * appears most often in the list.
 *
 * @param list
 *     The list of strings to obtain the commonest string from
 *     <b>Must not</b> be NULL.
 * @param ignore
 *     string value to ignore.
 *     <b>Must not</b> be NULL.
 * @returns
 *     The commonest string (one of the instances).  It has <i>not</i>
 *     been copied, so do not free() it.
 */
char *ll_list_mode (char **list, char *ignore);

/**
 * The ll_list_print function is used to print a list of strings to
 * the standard output.  If lptr is NULL, "(null)" will be printed,
 * otherwise the list contents will be printed within square brackets.
 * Each string will be printed in single quotes.
 *
 * @param lptr
 *     pointer to the list to be printed
 */
void ll_list_print (char **lptr);

/**
 * The ll_int_arrays_equal function is used to check if two arrays of
 * integers are equivalent.  It assumes:
 * - that the pointers are not NULL
 * - that the first element of the array is the length of the array
 *   (not including the size)
 *   For example, this is valid: {2, 1, 2}
 *
 * @param ra1
 *     Pointer to the first thing to compare
 * @param ra2
 *     Pointer to the second thing to compare
 */
int ll_int_arrays_equal (int* ra1, int* ra2);

/**
 * The ll_lists_equal function is used to check if two arrays of
 * char* elements are equivalent.  It assumes:
 * - that the pointers are not NULL
 * - that the lists end in a NULL
 *   For example, this is valid: {"your", "mom", NULL}
 *
 * @param list_one
 *     Pointer to the first thing to compare
 * @param list_two
 *     Pointer to the second thing to compare
 */
int ll_lists_equal(char **list_one, char **list_two);

#ifdef __cplusplus
}
#endif

#endif /* LIBLICENSE_H */
