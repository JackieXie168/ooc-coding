
/* This is a XmlTest class implementation file
 */

#include <ooc/testcase.h>

#include "../xml.h"

#ifdef OOC_NO_FINALIZE
#define ooc_finalize_class( x )
#define ooc_finalize_all( )
#endif


/** @class XmlTest
 *  @brief XmlTest class - brief description.
 * 	@see xmltest.h
 */

/** @file xmltest.h 
 *  @brief XmlTest class - brief description.
 * XmlTest is a .....
 * @note	This class is a final class, can not be inherited.
 * @note	Run as: valgrind --leak-check=yes --quiet ./xmltest
 */ 

DeclareClass( XmlTest, TestCase );

Virtuals( XmlTest, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( XmlTest, TestCase )

	int		data;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( XmlTest, TestCase );


/* Class initializing
 */

static void xmltest_before_class( XmlTest self );
static void xmltest_before( XmlTest self );
static void xmltest_after( XmlTest self );
static void xmltest_after_class( XmlTest self );

static
void
XmlTest_initialize( Class this )
{
	XmlTestVtable vtab = & XmlTestVtableInstance;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) xmltest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) xmltest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) xmltest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) xmltest_after_class;

	ooc_init_class( Exception );
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE

static
void
XmlTest_finalize( Class this )
{
	/* Release global resources! */
}

#endif

/* Constructor
 */

static
void
XmlTest_constructor( XmlTest self, const void * params )
{
	assert( ooc_isInitialized( XmlTest ) );
	
	chain_constructor( XmlTest, self, params );
}

/* Destructor
 */

static
void
XmlTest_destructor( XmlTest self, XmlTestVtable vtab )
{
}

/* Copy constuctor
 */

static
int
XmlTest_copy( XmlTest self, const XmlTest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
xmltest_before_class( XmlTest self )
{
	return;
}

static
void
xmltest_before( XmlTest self )
{
	return;
}

static
void
xmltest_after( XmlTest self )
{
	return;
}

static
void
xmltest_after_class( XmlTest self )
{
	return;
}


/*	=====================================================
	Test methods
 */

void
static
xmltest_method1( XmlTest self )
{
	XmlManager xmlm = xml_manager_new( XML_OUTPUT );

	xml_set_file( xmlm, stdout );

	xml_set_indent( xmlm, -1 );

	xml_write_begin_element( xmlm, "html" );

		xml_write_begin_element( xmlm, "head" );
			xml_write_begin_element( xmlm, "script" );
			xml_write_attribute( xmlm, "type", "text/javascript" );
			xml_write_element_text( xmlm, "szöveg", "Ez egy kuvra hosszú text!" );
			xml_write_text( xmlm, "Második text!" );
			xml_write_end_element( xmlm );
			xml_write_element_text( xmlm, "epilog", "Befejező szövegrész" );
		xml_write_end_element( xmlm );

		xml_write_begin_element( xmlm, "body" );
		xml_write_attribute( xmlm, "owner", "Joe" );
		xml_write_attribute( xmlm, "date", "Today" );
		xml_write_end_element( xmlm );

	xml_write_end_element( xmlm );

	fflush( stdout );
}

/** Test methods order table.
 * Put your test methods in this table in the order they should be executed
 * using the TEST(method) macro. 
 * 
 */
 
static ROM_ALLOC struct TestCaseMethod methods[] =
{
	
	TEST(xmltest_method1),
	
	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */
 
TESTCASE_MAIN
{
	XmlTest xmltest;
	int result;
	
	ooc_init_class( XmlTest );
	xmltest = ooc_new( XmlTest, &methods );
	result = testcase_run((TestCase)xmltest);
	ooc_delete( (Object) xmltest );
	ooc_finalize_all();
	return result;
}
