#ifndef OOC_H
#define OOC_H

#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef FALSE
#define FALSE   0
#define TRUE (! FALSE)
#endif

/* Include environment specific definitions
 */

#if defined( _MSC_VER )
	#include "port/msvc.h"
	
#elif defined( __GNUC__ )
	#include "port/gnuc.h"
	
#else
	#include "port/anyc.h"
#endif

/* Base objects and types
 */
 
/** Class description table.
 */

typedef const struct ClassTable * Class;

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
  
/** Creates a new object of a class.
 * Creates a new object of a class with the given construction parameters.
 * @param	pClass	The name of the class.
 * @param	pPar	Pointer to the construction parameters. This pointer is passed to the constructor without any check.
 * @return	The newly created Object.
 * @note	ooc_new may throw an Exception! This is a convenient macro for ooc_new_classptr().
 * @see		ooc_new_classptr()
 */
 
#define		ooc_new( pClass, pPar ) ooc_new_classptr( & pClass ## Class, pPar )

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
 
#define     ooc_isInstanceOf( pObj, pClass )  _ooc_isInstanceOf( pObj, & pClass ## Class )

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
 
int			_ooc_isInstanceOf( const void * object, const Class base );

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

void *		ooc_malloc( size_t size );					/**< Memory allocation. Allocates memory like malloc(), but throws an Exception on error */
void *		ooc_calloc( size_t num, size_t size );		/**< Memory allocation and clear. Allocates memory like calloc(), but throws an Exception on error */
void *		ooc_realloc( void *ptr, size_t size );		/**< Memory reallocation. Reallocates memory like realloc(), but throws an Exception on error */
void *		ooc_memdup( const void *ptr, size_t size );	/**< Memory duplication. Duplicates a memory block with a new alloc, throws an Exception on error */
char *		ooc_strdup( const char * s );				/**< C string duplication. Duplicates a C string with a new alloc, throws an Exception on error */
void 		ooc_free( const void * mem );				/**< Memory free. Frees memory allocated by ooc_malloc(), ooc_calloc() or ooc_realloc() */
void 		ooc_free_and_null( void ** mem );			/**< Memory free and nulling pointer. Frees memory via a pointer, and NULL the pointer thread safely */

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

/* Class definitions.
 */

typedef		  struct BaseVtable_stru * BaseVtable;

typedef              BaseVtable    Vtable;

enum ooc_CopyMode { OOC_COPY_DEFAULT = 0, OOC_COPY_DONE, OOC_NO_COPY };

struct ClassTable
{
	const size_t		size;				/* size of the object */
	const char *		name;				/* the name of the class (for information only) */
	const Class		 	parent;				/* parent of the class */
	Vtable			    vtable;				/* the pointer to the virtual function's table */
	const size_t		vtab_size;			/* the size of the vtable */
	
	void				(* init) ( Class this );        				/* class initializer */
	void				(* finz) ( Class this );						/* class finalizer */
	void				(* ctor) (Object self, const void * params );	/* constructor */
	void				(* dtor) (Object self, Vtable vtab);			/* destructor */
	int				  	(* copy) (Object self, const Object from); 		/* copy constructor */
};

struct BaseVtable_stru
{
	Class			_class;
	Class			_class_register_prev;
	Class			_class_register_next; 
	void   			(* _delete )( Object );
};

struct BaseObject											
{
	Vtable	_vtab;
};

extern const struct ClassTable BaseClass;


/*	Virtual functions access macro declaration
 */
#define _vtab_access_prototype( pClass )										\
			pClass ## Vtable pClass ## Virtual( pClass this )

#define _vtab_access_fn( pClass )												\
		_vtab_access_prototype( pClass )										\
			{ return ( pClass ## Vtable ) ( ((struct BaseObject *) this )->_vtab ); }

#define _parent_vtab_access_prototype( pClass, pParent )									\
			pParent ## Vtable pClass ## ParentVirtual( pClass this )

#define _parent_vtab_access_fn( pClass, pParent )										\
			_parent_vtab_access_prototype( pClass, pParent )	{							\
				assert ( ((struct BaseObject *) this )->_vtab->_class->parent != &BaseClass ); \
				return ( pParent ## Vtable ) ( ((struct BaseObject *) this )->_vtab->_class->parent->vtable ); \
				}

#if !defined( NO_INLINE )	/* Compilers that support function inlining */

#define _declare_vtab_access( pClass, pParent )									\
			STIN _vtab_access_fn( pClass )								\
			STIN _parent_vtab_access_fn( pClass, pParent )
#define _define_vtab_access( pClass, pParent )

#else				/* Compilers that does not support function inlining */

#define _declare_vtab_access( pClass, pParent )									\
			extern _vtab_access_prototype( pClass );					\
			extern _parent_vtab_access_prototype( pClass, pParent );
#define _define_vtab_access( pClass, pParent )									\
			_vtab_access_fn( pClass )									\
			_parent_vtab_access_fn( pClass, pParent )

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
	typedef struct pClass ## Object * pClass;				\
	extern const struct ClassTable pClass ## Class


/** Class virtual functions declaration macro.
 * This macro should be put int the publicly available header of the class.
 * Use:
 * @code
 * Virtuals( MyClass, Base )
 *     int   (* my_method) ( int parameter );
 * EndOfVirtuals;
 * @endcode
 * @param	pClass	The name of the class.
 * @param	pParent	The name of the parent class of the class. Must be @c Base if class does not have other parent.
 * @see		EndOfVirtuals
 * @hideinitializer
 */

#define Virtuals( pClass, pParent )												\
																				\
	/* defining inline function for virtual function access: */					\
	typedef struct pClass ## Vtable_stru * pClass ## Vtable;                    \
                                                                                \
	_declare_vtab_access( pClass, pParent )										\
																				\
	/* defining virtual function members */										\
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
	/* Prototyping */										\
	static void   pClass ## _initialize ( Class );	        \
	static void   pClass ## _finalize ( Class );	        \
	static void   pClass ## _constructor( pClass, const void * ); \
	static void   pClass ## _destructor ( pClass, pClass ## Vtable ); \
	static int	  pClass ## _copy ( pClass, const pClass );	\
															\
	/* Allocating the Vtable */								\
	struct pClass ## Vtable_stru pClass ## VtableInstance;	\
															\
	_define_vtab_access( pClass, pParent )					\
															\
	/* Allocating the class description table */			\
	const struct ClassTable pClass ## Class = {				\
		sizeof( struct pClass ## Object ),					\
		# pClass,											\
		& pParent ## Class,	                                \
		(Vtable) & pClass ## VtableInstance,				\
		sizeof( struct pClass ## Vtable_stru ),				\
											pClass ## _initialize,	\
											pClass ## _finalize,	\
		(void (*)( Object, const void *)) 	pClass ## _constructor,	\
		(void (*)( Object, Vtable ))        pClass ## _destructor,	\
		(int  (*)( Object, const Object)) 	pClass ## _copy	        \
		}
		
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
	
/*  Function marchaler types
 */

typedef	 void (* void_fn_voidp )( void * );	
typedef	 void (* void_fn_voidp_voidp )( void *, void * );
typedef	 int  (* int_fn_voidp_voidp  )( void *, void * );
typedef	 void (* void_fn_Object_voidp)( void *, void * );
typedef	 int  (* int_fn_Object_voidp )( void *, void * );

typedef void_fn_voidp ooc_destroyer;

#endif /* OOC_H */
