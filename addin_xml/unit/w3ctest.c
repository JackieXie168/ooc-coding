
/* This is a W3Ctest class implementation file
 */

#include <ooc/testcase.h>

#include "../xmlnode.h"


#ifdef OOC_NO_FINALIZE
#define ooc_finalize_class( x )
#define ooc_finalize_all( )
#endif


/** @class W3Ctest
 *  @brief W3Ctest class - a unit test class based on W3C test case recommendations
 * 	@see w3ctest.h
 */

/** @file w3ctest.h 
 *  @brief W3Ctest class - a unit test class based on W3C test case recommendations
 * W3Ctest is a test case that uses the W3C XML testsuite / test case recommendations as described at
 * http://www.w3.org/XML/Test/\n
 * The appropriate XML test suite must be downloaded.
 * @note	This class is a final class, can not be inherited.
 * @note	Run as: valgrind --leak-check=yes --quiet ./w3ctest
 */ 

DeclareClass( W3Ctest, TestCase );

Virtuals( W3Ctest, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( W3Ctest, TestCase )

	/* The test case */
	char *		testCaseFileName;
	FILE *		testCaseFile;
	XmlNode		testCaseXml;

	/* Individual tests */
	char *		testFileName;
	FILE *		testFile;
	XmlNode		parsedXml;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( W3Ctest, TestCase );


/* Class initializing
 */

static void w3ctest_before_class( W3Ctest self );
static void w3ctest_before( W3Ctest self );
static void w3ctest_after( W3Ctest self );
static void w3ctest_after_class( W3Ctest self );

static
void
W3Ctest_initialize( Class this )
{
	W3CtestVtable vtab = & W3CtestVtableInstance;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) w3ctest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) w3ctest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) w3ctest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) w3ctest_after_class;

	ooc_init_class( Exception );
	ooc_init_class( XmlNode );
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE

static
void
W3Ctest_finalize( Class this )
{
}

#endif

/* Constructor
 */

static
void
W3Ctest_constructor( W3Ctest self, const void * params )
{
	assert( ooc_isInitialized( W3Ctest ) );
	
	chain_constructor( W3Ctest, self, NULL );

	self->testCaseFileName = ooc_strdup( (char *) params );
}

/* Destructor
 */

static
void
W3Ctest_destructor( W3Ctest self, W3CtestVtable vtab )
{
	ooc_free_and_null( (void**) & self->testCaseFileName );
}

/* Copy constuctor
 */

static
int
W3Ctest_copy( W3Ctest self, const W3Ctest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */

#define BIGGEST_W3C_FILESIZE 256000L
static
void
w3ctest_before_class( W3Ctest self )
{
	unsigned long int 	fileSize;
	char *				buffer;
	size_t				bytes_read;
	XmlParser			parser;


	self->testCaseFile = fopen( self->testCaseFileName, "rb" );

	assertNotNullMsg( self->testCaseFile, "Can not open w3c XML test case file.");
	if( self->testCaseFile != NULL )
	{
		fseek( self->testCaseFile, 0L, SEEK_END );
		fileSize = ftell( self->testCaseFile );
		rewind( self->testCaseFile );

		assertTrueMsg( fileSize <= BIGGEST_W3C_FILESIZE, "w3c test case file is too big!" );
		assertTrueMsg( fileSize > 0, "w3c test case file is empty!" );
		if( fileSize <= BIGGEST_W3C_FILESIZE && fileSize > 0 )
		{
			buffer = ooc_malloc( fileSize + 1 );
			bytes_read = fread( buffer, 1, fileSize, self->testCaseFile );
			buffer[bytes_read] = '\0';

			parser = xmlparser_new_str( buffer );
			{
				ooc_manage_object( parser );
				self->testCaseXml = xmlnode_parse( parser );
				ooc_delete( (Object) ooc_pass( parser ) );
			}
		}
	}
}

static
void
w3ctest_before( W3Ctest self )
{
	return;
}

static
void
w3ctest_after( W3Ctest self )
{
	return;
}

static
void
w3ctest_after_class( W3Ctest self )
{
	FILE * tmpFile;

	tmpFile = ooc_ptr_read_and_null( (void**) & self->testCaseFile );
	if( tmpFile != NULL )
		fclose( tmpFile );
	ooc_delete_and_null( (Object*) & self->testCaseXml );
}

/* ===========================================================
 * Test cycles
 */

static
void
w3ctest_testcycle( W3Ctest self )
{
	printf( "w3c test cycle is running\n");
}

static
void
w3ctest_run_all( W3Ctest self )
{
	/* testcase_run_test( (TestCase) self, "kukurutty", (test_method_type) w3ctest_testcycle ); */
}

/* Runs the test as an executable
 */
 
TESTCASE_MAIN
{
	W3Ctest w3ctest;
	int result;
	
	ooc_init_class( W3Ctest );

	w3ctest = ooc_new( W3Ctest, "xmltest/xmltest_mod.xml" );

	if( testcase_run_before_class( (TestCase) w3ctest ) == 0 )
		w3ctest_run_all( w3ctest );
	result = testcase_run_after_class( (TestCase) w3ctest);

	ooc_delete( (Object) w3ctest );
	ooc_finalize_all();
	return result;
}
