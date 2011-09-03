/*
 * ooc.h
 * 
 * Copyright (c) 2011, old_fashioned. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */


#ifndef OOC_H
#define OOC_H

#include <stddef.h>

#ifndef FALSE
#define FALSE   0
#define TRUE (! FALSE)
#endif

/* Include environment specific definitions
 */

#if defined( _MSC_VER )
	#include "port/msvc.h"
	
#elif defined( __C30__ )
	#include "port/mcc30.h"

#elif defined( __PIC32MX__ )
	#include "port/mcc32.h"
	
#elif defined( __GNUC__ )
	#include "port/gnuc.h"
	
#elif defined( __18CXX  )
	#include "port/mcc18.h"
	
#else
	#include "port/anyc.h"
#endif

/* Type qualifiers
 */

#ifndef ROM
#define ROM const
#endif

#ifndef ROM_ALLOC
#define ROM_ALLOC ROM
#endif

/* Base objects and types
 */
 
/** Class description table.
 */

typedef ROM struct ClassTable * Class;

/** ooc Object.
 */
 
typedef		  struct BaseObject * Object;

/* Object and class manipulation functions
 */
 
/** @name Class initialization, finalization.
 * @nosubgrouping
 */
 
/*@{*/

/** Initializes a class.
 * Initializes a class using the class name. This is a convenient macro.
 * @param pClass	Tha name of the class.
 * @see _ooc_init_class().
 */
 
#define		ooc_init_class( pClass ) _ooc_init_class( & pClass ## Class )

/** Initializes a class by class table pointer.
 * Initializes the class pointed by the parameter.
 * @param class_ptr	Pointer to the class description table.
 * @see ooc_init_class() for more convenient use.
 */

void		_ooc_init_class( const Class class_ptr );

#ifndef OOC_NO_FINALIZE	

/** Finalizes a class.
 * Finalizes a class using the class name. This is a convenient macro.
 * @param pClass	Tha name of the class.
 * @see _ooc_finalize_class().
 */
 
#define		ooc_finalize_class( pClass ) _ooc_finalize_class( & pClass ## Class )

/** Finalizes a class by the class table pointer.
 * Finalizes a class using the class table pointer.
 * @param class_ptr	Pointer to the class description table.
 * @see ooc_finalize_class() for more convenient use.
 */
 
void		_ooc_finalize_class( const Class class_ptr );

/** Finalizes all classes.
 */
 
void		ooc_finalize_all( void );

#endif /* OOC_NO_FINALIZE */	

/** Checks if Class is initialized.
 * This is a convenient macro.
 * @param	pClass	The Class that should be checked.
 * @return	@c TRUE or @c FALSE. Does not throw an Exception.
 * @see		_ooc_isInitialized()
 */
 
#define     ooc_isInitialized( pClass ) _ooc_isInitialized( & pClass ## Class )

/** Checks if Class is initialized via a class table pointer.
 * This is a convenient macro.
 * @param	class_ptr	The class table pointer to the Class that should be checked.
 * @return	@c TRUE or @c FALSE.
 * @see		ooc_isInitialized() for more convenient use. 
 */
 
int			_ooc_isInitialized( const Class class_ptr );

/*@}*/

/** @name Creating and deleting Objects.
 */

/*@{*/
  
#ifndef OOC_NO_DYNAMIC_MEM

/** Creates a new object of a class.
 * Creates a new object of a class with the given construction parameters.
 * @param	pClass	The name of the class.
 * @param	pPar	Pointer to the construction parameters. This pointer is passed to the constructor without any check.
 * @return	The newly created Object.
 * @note	ooc_new may throw an Exception! This is a convenient macro for ooc_new_classptr().
 * @see		ooc_new_classptr()
 */
 
#define		ooc_new( pClass, pPar ) ((pClass) ooc_new_classptr( & pClass ## Class, pPar ))

/** Creates a new object of a class using class table pointer.
 * Creates a new object of a class with the given class table pointer and construction parameters.
 * @param	class_ptr	The pointer to the class description table.
 * @param	par_ptr		Pointer to the construction parameters. This pointer is passed to the constructor without any check.
 * @return	The newly created Object.
 * @note	ooc_new may throw an Exception!
 * @see		ooc_new_classptr() for more convenient use.
 */

Object		ooc_new_classptr( const Class class_ptr, const void * par_ptr );

/**	Duplicates an Object.
 * Calling the copy constructor of the @c Object, creates a duplicate of it.
 * @param	object	The original Object.
 * @return	The newly created Object.
 * @note	May throw an Exception.
 * @warning If in a multi-threaded application your @c Object needs to be in a consistent
 * state while copying, you must lock it yourself. @c ooc_duplicate() does not do any locking.
 */

Object		ooc_duplicate( const Object object );

/** Deletes an Object.
 * Deletes the Object using its destructor, then deallocating the memory.
 * @param	object	The Object to be deleted. Can be NULL.
 */
 
void		ooc_delete( Object object );

/** Deletes an Object via a pointer with nulling.
 * Deletes the Object using its destructor, then deallocating the memory.
 * Thread safely and reentrat safely NULLs the pointer.
 * @param	object_ptr	Pointer to the Object to be deleted. Can be NULL and the object pointer can be NULL as well.
 * @note	Use this version in class destructors! This is important to ensure the reentrancy for destructors.
 */

void		ooc_delete_and_null( Object * object_ptr );

#endif /* OOC_NO_DYNAMIC_MEM */

/** Creates an Object of a Class using existing memory block.
 * Creates a new object of a class with the given construction parameters using the memory block provided.
 * This method does not call the memory allocator for the Object
 * @param	mem		The memory block to be used for the Object
 * @param	pClass	The name of the class.
 * @param	pPar	Pointer to the construction parameters. This pointer is passed to the constructor without any check.
 * @return	Nothing.
 * @note	ooc_use may throw an Exception! This is a convenient macro for ooc_use_classptr().
 * @see		ooc_use_classptr() 
 * @warning	The caller must ensure that the memory block pointed by mem is large enough to hold the Object!
 */
  
#define		ooc_use( mem, pClass, pPar ) ooc_use_classptr( mem, & pClass ## Class, pPar )

/** Creates an Object from a class table pointer using existing memory block.
 * Creates a new object of a class with the given construction parameters using the memory block provided.
 * This method does not call the memory allocator for the Object
 * @param	mem			The memory block to be used for the Object
 * @param	class_ptr	The class table pointer.
 * @param	param		Pointer to the construction parameters. This pointer is passed to the constructor without any check.
 * @return	Nothing.
 * @note	ooc_use_classptr may throw an Exception!
 * @see		ooc_use() for more convenient use.
 * @warning	The caller must ensure that the memory block pointed by mem is large enough to hold the Object!
 */
  
void		ooc_use_classptr( void * mem, const Class class_ptr, const void * param );

/**	Copies an Object to an another location.
 * Calling the copy constructor of the @c Object, copies the content into an other location.
 * @param	to		The destination location of the copied object. The caller must ensure, that
 * 					the destination location is large enough to hold the newly created Object!
 * @param	from	The original Object to be copied.
 * @return	The newly created Object.
 * @note	May throw an Exception.
 * @warning If in a multi-threaded application your @c Object needs to be in a consistent
 * state while copying, you must lock it yourself. @c ooc_copy() does not do any locking.
 */

Object		ooc_copy( void * to, const Object from );

/** Destroys an Object, but does not free the memory.
 * Calls the Object's destructor, but does not free the memory block used by the Object.
 * @param	object		The Object to be released.
 * @see		ooc_use()
 */
 
void		ooc_release( Object object );

/*@}*/

/** @name Dynamic type checking.
 * Helpers to check the Object types in run-time.
 */
 
/*@{*/

/** Checks if a Class is inherited from an other Class.
 * This is a convenient macro. 
 * @param	pClass			The Class that should be checked.
 * @param	pSuperClass		The required type
 * @return	@c TRUE or @c FALSE. Returns @c TRUE if the Class is inherited from the given pSuperClass.
 * 			Does not throw an Exception.
 * @see		_ooc_isClassOf()
 */
 
#define     ooc_isClassOf( pClass, pSuperClass )  _ooc_isClassOf( & pClass ## Class, & pSuperClass ## Class )

/** Checks if an Object is of a given type.
 * This is a convenient macro. Returns @c TRUE if the Object is of the given Class or any of its parent classes.
 * @param	pObj	The Object to be checked.
 * @param	pClass	The Class that should be matched.
 * @return	@c TRUE or @c FALSE. Does not throw an Exception.
 * @see		_ooc_isInstanceOf()
 */
 
#define     ooc_isInstanceOf( pObj, pClass )  _ooc_isInstanceOf( (Object) pObj, & pClass ## Class )

/** Run-time safe upcast of an Object.
 * Safely upcasts the Object to the specified type. Throws an Exception if not possible. This is a macro.
 * @param	pObj	The Object to be cast.
 * @param	pClass	The desired type (Class name).
 * @return	The Objcet as a new type.
 * @see		ooc_check_cast()
 */
 
#define     ooc_cast( pObj, pClass )  ( ooc_check_cast( pObj, & pClass ## Class ), (pClass) pObj )

/** Gets the type of the Object in run-time.
 * @param	object	The Object.
 * @return	The Object's type: a pointer to the class table.
 * @note	Throws an Exception if @c object is not a valid Object.
 */
 
Class		ooc_get_type( const Object object );
 
/** Checks if a Class has a parent class.
 * This is a convenient macro. 
 * @param	this		The Class that should be checked.
 * @return	@c TRUE or @c FALSE. Returns @c TRUE if the Class has a parent class.
 * 			Does not throw an Exception.
 */
 
#define ooc_class_has_parent(this) (this->parent != &BaseClass)

/** Checks if a Class is inherited from an other Class via class table pointers
 * This is a convenient macro. 
 * @param	this		The Class that should be checked.
 * @param	base		The required type
 * @return	@c TRUE or @c FALSE. Returns @c TRUE if the Class is inherited from the given pSuperClass.
 * 			Does not throw an Exception.
 * @see		ooc_isClassOf()
 */
 
int			_ooc_isClassOf( const Class this, const Class base );

/** Checks if an Object is of a given type via class table pointer.
 * Returns @c TRUE if the Object is of the given Class or any of its parent classes.
 * @param	object	The Object to be checked.
 * @param	base	The Class that should be matched.
 * @return	@c TRUE or @c FALSE. Does not throw an Exception.
 * @see		ooc_isInstanceOf() for more convenient use.
 */
 
int			_ooc_isInstanceOf( const Object object, const Class base );

/** Run-time safe check, if an Object can be upcast to an other type.
 * Safely chaecks if the Object could be upcast to the specified type.
 * Throws an Exception if not possible.
 * @param	object	The Object to be cast.
 * @param	target	The desired type (Class name).
 * @return	Just returns, or throws an Exception if casting is not possible. 
 * @see		ooc_cast()
 */
 
void		ooc_check_cast( void * object, const Class target );

/*@}*/

/** @name Memory handling.
 * ooc has it own memory handling that are based on their own stdc equivalents,
 * except that they do not return a failure code, but throw an Exception instead.
 * @nosubgrouping
 */
/*@{*/

#ifndef OOC_NO_DYNAMIC_MEM

void *		ooc_malloc( size_t size );					/**< Memory allocation. Allocates memory like malloc(), but throws an Exception on error */
void *		ooc_calloc( size_t num, size_t size );		/**< Memory allocation and clear. Allocates memory like calloc(), but throws an Exception on error */
void *		ooc_realloc( void *ptr, size_t size );		/**< Memory reallocation. Reallocates memory like realloc(), but throws an Exception on error */
void *		ooc_memdup( const void *ptr, size_t size );	/**< Memory duplication. Duplicates a memory block with a new alloc, throws an Exception on error */
char *		ooc_strdup( const char * s );				/**< C string duplication. Duplicates a C string with a new alloc, throws an Exception on error */
void 		ooc_free( void * mem );						/**< Memory free. Frees memory allocated by ooc_malloc(), ooc_calloc() or ooc_realloc() */
void 		ooc_free_and_null( void ** mem );			/**< Memory free and nulling pointer. Frees memory via a pointer, and NULL the pointer thread safely */

#endif

/** Pointer read and null.
 * Reads a pointer via a pointer, and nulls thread safely.
 * Atomic operation in case of GNUC and MSVC on x86 platforms.
 * For other compilers this is equuivalent to the following code:
 * @code
 * void * tmp = * ptr_ptr;
 * *ptr_ptr = NULL;
 * return tmp;
 * @endcode
 * wich is not thread safe but re-entrant safe.
 */ 

void * 		ooc_ptr_read_and_null( void ** ptr_ptr );

/*@}*/

/* Interface definitions
 */

/**	Interface ID.
 * 	Each interface has a unique ID.\n
 * 	In this implementation the ID is a pointer to an interafce descriptor table in the memory,
 *  similarly to the Class definition.
 *  @hideinitializer
 */
typedef ROM struct InterfaceID_struct * InterfaceID;

struct InterfaceOffsets_struct
{
	InterfaceID		id;			/* The interface ID of the implemented interface */
	size_t			offset;		/* The offset of the interface in the class's virtual table */
};

typedef ROM struct InterfaceOffsets_struct * Itable;

/* Class definitions.
 */

typedef		  struct BaseVtable_stru * BaseVtable;

typedef              BaseVtable    Vtable;

enum ooc_CopyMode { OOC_COPY_DEFAULT = 0, OOC_COPY_DONE, OOC_NO_COPY };

struct ClassTable
{
	const size_t		size;				/* size of the object */
	ROM   char *		name;				/* the name of the class (for information only) */
	ROM   Class		 	parent;				/* parent of the class */
	Vtable			    vtable;				/* the pointer to the virtual function's table */
	const size_t		vtab_size;			/* the size of the vtable */
	Itable				itable;				/* the implemented interfaces' Itable */
	const size_t		itab_size;			/* the number of implemented Interfaces */
	

	void				(* init) ( Class this );        				/* class initializer */
#ifndef OOC_NO_FINALIZE	
	void				(* finz) ( Class this );						/* class finalizer */
#endif
	void				(* ctor) (Object self, const void * params );	/* constructor */
	void				(* dtor) (Object self, Vtable vtab);			/* destructor */
	int				  	(* copy) (Object self, const Object from); 		/* copy constructor */
};

struct BaseVtable_stru
{
	Class			_class;
#ifndef OOC_NO_FINALIZE	
	Class			_class_register_prev;
	Class			_class_register_next; 
#endif
	int   			(* _destroy_check )( Object );
};

struct BaseObject											
{
	Vtable	_vtab;
};

extern ROM struct ClassTable BaseClass;


/*	Virtual functions access macro declaration
 */
#define _vtab_access_prototype( pClass )										\
			pClass ## Vtable pClass ## Virtual( pClass this )

#define _vtab_access_fn( pClass )												\
		_vtab_access_prototype( pClass )										\
			{ return ( pClass ## Vtable ) ( ((struct BaseObject *) this )->_vtab ); }

#define _parent_vtab_access_prototype( pClass, pParent )						\
			pParent ## Vtable pClass ## ParentVirtual( pClass this )

#define _parent_vtab_access_fn( pClass, pParent )								\
			_parent_vtab_access_prototype( pClass, pParent )	{				\
				assert ( ((struct BaseObject *) this )->_vtab->_class->parent != &BaseClass ); \
				return ( pParent ## Vtable ) ( ((struct BaseObject *) this )->_vtab->_class->parent->vtable ); \
				}

#if !defined( NO_INLINE )	/* Compilers that support function inlining */

#define _declare_vtab_access( pClass, pParent )									\
			STIN _vtab_access_fn( pClass )										\
			STIN _parent_vtab_access_fn( pClass, pParent )
#define _define_vtab_access( pClass, pParent )

#else				/* Compilers that does not support function inlining */

#define _declare_vtab_access( pClass, pParent )									\
			extern _vtab_access_prototype( pClass );							\
			extern _parent_vtab_access_prototype( pClass, pParent );
#define _define_vtab_access( pClass, pParent )									\
			_vtab_access_fn( pClass )											\
			_parent_vtab_access_fn( pClass, pParent )

#endif

#ifndef _OOC_VTAB_INITIALIZER
#define _OOC_VTAB_INITIALIZER
#endif

/** @name Class definitions.
 * These macros help define the class.
 * @nosubgrouping
 */

/*@{*/

/** Class declaration macro.
 * This macro should be put int the publicly available header of the class.
 * Use:
 * @code
 * DeclareClass( MyClass, Base );
 * @endcode
 * @param	pClass	The name of the class.
 * @param	pParent	The name of the parent class of the class. Must be @c Base if class does not have other parent.
 * @hideinitializer
 */

#define	DeclareClass( pClass, pParent )  							\
	typedef struct pClass ## Object * pClass;						\
	extern ROM struct ClassTable pClass ## Class


/** Class virtual functions declaration macro.
 * This macro should be put int the publicly available header of the class.
 * Use:
 * @code
 * Virtuals( MyClass, Base )
 *     int   (* my_method) ( MyClass self, int param );
 * EndOfVirtuals;
 * @endcode
 * @param	pClass	The name of the class.
 * @param	pParent	The name of the parent class of the class. Must be @c Base if class does not have other parent.
 * @see		EndOfVirtuals
 * @hideinitializer
 */

#define Virtuals( pClass, pParent )												\
																				\
	typedef struct pClass ## Vtable_stru * pClass ## Vtable;                    \
                                                                                \
	_declare_vtab_access( pClass, pParent )										\
																				\
	struct pClass ## Vtable_stru {												\
		struct pParent ## Vtable_stru	pParent;
		
/** End of virtual functions.
 * This macro terminates the @c Virtuals block.
 * @see		Virtuals()
 * @hideinitializer
 */

#define EndOfVirtuals	}

/** Class members declaration macro.
 * This macro should be put int the implementation header of the class.
 * Use:
 * @code
 * ClassMembers( MyClass, Base )
 *     int   my_data;
 * EndOfClassMembers;
 * @endcode
 * @param	pClass	The name of the class.
 * @param	pParent	The name of the parent class of the class. Must be @c Base if class does not have other parent.
 * @see		EndOfClassMembers
 * @hideinitializer
 */

#define ClassMembers( pClass, pParent )						\
	struct pClass ## Object {								\
		struct pParent ## Object pParent;				

/** End of class members definition.
 * This macro terminates the @c ClassMembers block.
 * @see		ClassMembers()
 * @hideinitializer
 */
#define EndOfClassMembers	}


#ifndef OOC_NO_FINALIZE

/** Class allocation macro.
 * This macro should be put int the implementation file of the class.
 * Use:
 * @code
 * AllocateClass( MyClass, Base );
 * @endcode
 * @param	pClass	The name of the class.
 * @param	pParent	The name of the parent class of the class. Must be @c Base if class does not have other parent.
 * @hideinitializer
 */

#define AllocateClass( pClass, pParent )					\
															\
	static void   pClass ## _initialize ( Class );	        \
	static void   pClass ## _finalize ( Class );	        \
	static void   pClass ## _constructor( pClass, const void * ); \
	static void   pClass ## _destructor ( pClass, pClass ## Vtable ); \
	static int	  pClass ## _copy ( pClass, const pClass );	\
															\
	static struct pClass ## Vtable_stru pClass ## VtableInstance _OOC_VTAB_INITIALIZER; \
															\
	_define_vtab_access( pClass, pParent )					\
															\
	ROM_ALLOC												\
	struct ClassTable pClass ## Class = {					\
		sizeof( struct pClass ## Object ),					\
		(ROM char *) #pClass,								\
		& pParent ## Class,	                                \
		(Vtable) & pClass ## VtableInstance,				\
		sizeof( struct pClass ## Vtable_stru ),				\
		(Itable) NULL, 										\
		(size_t) 0,											\
												pClass ## _initialize,	\
												pClass ## _finalize,	\
		(void (*)( Object, const void *)) 		pClass ## _constructor,	\
		(void (*)( Object, Vtable ))        	pClass ## _destructor,	\
		(int  (*)( Object, const Object)) 		pClass ## _copy,        \
		}

/** Class allocation macro for classes with interfaces.
 * Allocates the class using the InterfaceRegister for the given class.
 * The InterfaceRegister must be put right before this class allocator.
 * For proper use see InterfaceRegister.
 * @param	pClass	The name of the class.
 * @param	pParent	The name of the parent class of the class. Must be @c Base if class does not have other parent.
 * @see		InterfaceRegister, AllocateClass
 * @hideinitializer
 */

#define AllocateClassWithInterface( pClass, pParent )		\
															\
	static void   pClass ## _initialize ( Class );	        \
	static void   pClass ## _finalize ( Class );	        \
	static void   pClass ## _constructor( pClass, const void * ); \
	static void   pClass ## _destructor ( pClass, pClass ## Vtable ); \
	static int	  pClass ## _copy ( pClass, const pClass );	\
															\
	static struct pClass ## Vtable_stru pClass ## VtableInstance _OOC_VTAB_INITIALIZER; \
															\
	_define_vtab_access( pClass, pParent )					\
															\
	ROM_ALLOC												\
	struct ClassTable pClass ## Class = {					\
		sizeof( struct pClass ## Object ),					\
		(ROM char *) #pClass,								\
		& pParent ## Class,	                                \
		(Vtable) & pClass ## VtableInstance,				\
		sizeof( struct pClass ## Vtable_stru ),				\
		pClass ## Itable,									\
		sizeof(pClass ## Itable)/sizeof(struct InterfaceOffsets_struct),\
												pClass ## _initialize,	\
												pClass ## _finalize,	\
		(void (*)( Object, const void *)) 		pClass ## _constructor,	\
		(void (*)( Object, Vtable ))        	pClass ## _destructor,	\
		(int  (*)( Object, const Object)) 		pClass ## _copy,        \
		}

#else

#define AllocateClass( pClass, pParent )					\
															\
	static void   pClass ## _initialize ( Class );	        \
	static void   pClass ## _constructor( pClass, const void * ); \
	static void   pClass ## _destructor ( pClass, pClass ## Vtable ); \
	static int	  pClass ## _copy ( pClass, const pClass );	\
															\
	static struct pClass ## Vtable_stru pClass ## VtableInstance _OOC_VTAB_INITIALIZER; \
															\
	_define_vtab_access( pClass, pParent )					\
															\
	ROM_ALLOC												\
	struct ClassTable pClass ## Class = {					\
		sizeof( struct pClass ## Object ),					\
		(ROM char *) #pClass,								\
		& pParent ## Class,	                                \
		(Vtable) & pClass ## VtableInstance,				\
		sizeof( struct pClass ## Vtable_stru ),				\
		(Itable) NULL, 										\
		(size_t) 0,											\
											pClass ## _initialize,	\
		(void (*)( Object, const void *)) 	pClass ## _constructor,	\
		(void (*)( Object, Vtable ))        pClass ## _destructor,	\
		(int  (*)( Object, const Object)) 	pClass ## _copy	        \
		}

#define AllocateClassWithInterface( pClass, pParent )		\
															\
	static void   pClass ## _initialize ( Class );	        \
	static void   pClass ## _constructor( pClass, const void * ); \
	static void   pClass ## _destructor ( pClass, pClass ## Vtable ); \
	static int	  pClass ## _copy ( pClass, const pClass );	\
															\
	static struct pClass ## Vtable_stru pClass ## VtableInstance _OOC_VTAB_INITIALIZER; \
															\
	_define_vtab_access( pClass, pParent )					\
															\
	ROM_ALLOC												\
	struct ClassTable pClass ## Class = {					\
		sizeof( struct pClass ## Object ),					\
		(ROM char *) #pClass,								\
		& pParent ## Class,	                                \
		(Vtable) & pClass ## VtableInstance,				\
		sizeof( struct pClass ## Vtable_stru ),				\
		pClass ## Itable,									\
		sizeof(pClass ## Itable)/sizeof(struct InterfaceOffsets_struct),\
												pClass ## _initialize,	\
		(void (*)( Object, const void *)) 		pClass ## _constructor,	\
		(void (*)( Object, Vtable ))        	pClass ## _destructor,	\
		(int  (*)( Object, const Object)) 		pClass ## _copy,        \
		}

#endif /* OOC_NO_FINALIZE */

		
/** Chain the parent constructor.
 * This macro calls the constructor of the parent class.
 * Must be used soley in the constructor of the class, at the beginning of the constructor.
 * Use:
 * @code
 * static void MyClass_constructor( MyClass self, void * params )
 * {
 * 		assert( ooc_isInitialized( MyClass ) );
 * 
 * 		chain_constructor( MyClass, self, NULL );
 * 
 * 		.... other construction code here
 * }
 * @endcode
 * @param	pClass	The name of the actual class (not the parent!)
 * @param	pSelf	Pointer to the @c Object being constructed.
 * @param	pParam	Pointer to the construction parameters for the parent class's constructor.
 * 					This pointer is passed to the constructor without any check.
 * @note In some rare cases you may neglect calling the parent constructor, e.g. when there is no parent class,
 * or if you definitly know, that the parent class does not need construction, like in case of @c ListNode.
 * @hideinitializer
 */

#define chain_constructor( pClass, pSelf, pParam ) \
	if( pClass ## Class.parent != &BaseClass ) pClass ## Class.parent->ctor( (Object) pSelf, pParam )

/*@}*/

/** @name Managing Interfaces.
 * These macros help define and use the interfaces and mixins.
 * @nosubgrouping
 */

/*@{*/

struct
InterfaceID_struct
{
	char dummy;					/* Just a space holder to ensure that all InterfaceID is unique */
};

/**	Declare an interface.
 * This declaration should be placed in a publicly available header file. The declaration is followed by a semicolon
 * separated list of function pointers. These function pointers represent the behavior of the interface.
 * The first parameters of the function pointers should be an Object, since we do not know the exact type of the
 * object right now, that implements the interface.
 * @code
 * DeclareInterface( Printable )
 *     void		(* print) ( Object );
 *     int		(* set_header) ( Object, const char * );
 *     ...
 * EndOfInterface;
 * @endcode
 * @param	pInterface	The name of the interface.
 * @see		EndOfInterface
 * @hideinitializer
 */

#define DeclareInterface( pInterface )						\
	extern ROM struct InterfaceID_struct pInterface ## ID;	\
	typedef struct pInterface ## Methods * pInterface;		\
	struct pInterface ## Methods {

/**	Terminates the interface declaration.
 * @see		DeclareInterface
 * @hideinitializer
 */

#define EndOfInterface }

/**	Place an interface into the class's virtual table.
 * This macro places the interface methods into the class's virtual table.
 * This marks that he interface methods can be accessed via the virtual table,
 * and are inherited as any normal virtual function.
 * Use:
 * @code
 * Virtuals( MyClass, Base )
 *		int   (* my_method) ( MyClass self, int param );
 *			...
 * 		Interface( HasProperties );
 * 		Interface( Printable );
 * EndOfVirtuals;
 * @endcode
 * @param	pInterface	The name of the interface to be implemented by the class.
 * @hideinitializer
 */

#define Interface( pInterface )								\
	struct pInterface ## Methods pInterface

/**	Place the interface data fields into the class members.
 * This macro places the interface data fields (in case of Mixins) into the object struct.
 * Use:
 * @code
 * ClassMembers( IceCream, Tonic )
 *		int			scoop;
 *		InterfaceData( Flavour );
 * EndOfClassMembers;
 * @endcode
 * @param	pInterface	The name of the interface to be implemented by the class.
 * @note	This macro puts the interface data into the Object struct as private! The enclosing
 * 			class can not access the members of the mixed in class with a simple pointer dereferencing.
 * @hideinitializer
 */

#define InterfaceData( pInterface )							\
		char pInterface[ sizeof( pInterface ## DataFields )/sizeof(char) ];


/**	Allocates the interface descriptor table.
 * The interface descriptor table is used to uniquely identify an interface.
 * This macro allocates this table and must be placed in a publicly available source file.
 * In larger projects all interface descriptor table allocators can be put in a single file
 * (e.g. in interfaces.c ).
 * @param	pInterface	The name of the interface to be allocated.
 * @see 	InterfaceID
 * @hideinitializer
 */

#define AllocateInterface( pInterface )						\
	ROM_ALLOC struct InterfaceID_struct pInterface ## ID

/**	Register for implemented interfaces of the class.
 * In the class implementation code you must define, which interfaces are implemented by the class.
 * These interfaces must be listed in the InterfaceRegister right before the AllocateClassWithInterface macro.
 * Typical use (in myclass.c):
 * @code
 * InterfaceRegister( MyClass )
 * {
 * 		AddInterface( MyClass, HasProperties ),
 * 		AddInterface( MyClass, Printable )
 * };
 *
 * AllocateClassWithInterface( MyClass, Base );
 * @endcode
 * @param	pClass	The name of the class that implements the listed interfaces.
 * @note	Listen to the different syntax! Internally this is a table of structs, so you must end it with a semicolon,
 * 			and use comma as the internal list separator. You must not put a comma after the last item in the list!
 * @see 	AddInterface, AllocateClassWithInterface
 * @warning	Using AllocateClass instead of AllocateClassWithInterface will not put the registered interfaces into the
 * 			class table, thus none of the interfaces could be used! Always use AllocateClassWithInterface after an InterfaceRegister!
 * @hideinitializer
 */

#define InterfaceRegister( pClass )							\
	static ROM_ALLOC										\
	struct InterfaceOffsets_struct pClass ## Itable[] =

/**	Adds an interface to the class's interface register.
 * @param	pClass		The name of the class that implements the interfaces.
 * @param	pInterface	The name of the interface to be implemented for the class.
 * @see 	InterfaceRegister
 * @hideinitializer
 */

#define AddInterface( pClass, pInterface )					\
	{ & pInterface ## ID, offsetof( struct pClass ## Vtable_stru, pInterface ) }

/**	Retrieves an interface of the Object.
 * Returns the interface pointer for the given Object.
 * Typical use:
 * @code
 * void
 * print_if_printable( Object object )
 * {
 * 		Printable printable = get_interface( object, Printable );
 *
 * 		if( printable )
 * 			printable->print( object );
 * }
 * @endcode
 * @param	pObject		The Object of which's interface we are interested in.
 * @param	pInterface	The name of the interface to be retrieved.
 * @return	The interface (pointer to the interface methods table), or NULL if
 * 			this interface is not implemented for the class.
 * @see 	ooc_get_interface_must_have, DeclareInterface
 * @hideinitializer
 */

#define ooc_get_interface( pObject, pInterface )			\
	( (pInterface) _ooc_get_interface( (Object) pObject, & pInterface ## ID ) )

/**	Retrieves a mandatory interface of the Object.
 * Returns the interface pointer for the given Object.
 * The Interface must have been implemented for the given Object, otherwise
 * @c err_interface_not_implemented Exception is thrown.
 * @param	pObject		The Object of which's interface we are interested in.
 * @param	pInterface	The name of the interface to be retrieved.
 * @return	The interface (pointer to the interface methods table), never returns NULL.
 * 			Throws an Exception with error code of err_interface_not_implemented
 * 			if the interface is not implemented for this class.
 * @see 	ooc_get_interface
 * @hideinitializer
 */

#define ooc_get_interface_must_have( pObject, pInterface )			\
	( (pInterface) _ooc_get_interface_must_have( (Object) pObject, & pInterface ## ID ) )

/*@}*/

void * _ooc_get_interface( const Object, InterfaceID );

void * _ooc_get_interface_must_have( const Object, InterfaceID );

/*  Function marchaler types
 */

typedef	 void (* void_fn_voidp )( void * );	
typedef	 void (* void_fn_voidp_voidp )( void *, void * );
typedef	 int  (* int_fn_voidp_voidp  )( void *, void * );
typedef	 void (* void_fn_Object_voidp)( void *, void * );
typedef	 int  (* int_fn_Object_voidp )( void *, void * );

typedef void_fn_voidp ooc_destroyer;

/** Container item management flag. 
 * Indicates that the contents of a container must be managed by the container itself.
 * @see vector_new_type(), list_new_type()
 */
 
#define OOC_MANAGE TRUE

/* Memmory handling macros
 */

#ifndef GEN_PTR
#define GEN_PTR char *
#endif


#endif /* OOC_H */
