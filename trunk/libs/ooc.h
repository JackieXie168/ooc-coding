#ifndef OOC_H
#define OOC_H

#include <stdlib.h>
#include <setjmp.h>
#include <assert.h>

#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef FALSE
#define FALSE   0
#define TRUE (! FALSE)
#endif


/* implementing static inline */
#if defined( _MSC_VER )
#define STIN static _inline
#elif defined( __GNUC__ ) && !defined( __NO_INLINE__ )
#define STIN static inline
#else 
#define STIN static
#warning "inline functions are unavailabe"
#endif

/* implementing Thread Local Storage */
#if defined( _MSC_VER )
#define TLS static _declspec( thread )
#elif defined( __GNUC__ )
#define TLS static __thread
#else
#define TLS
#warning "ooc implementation is not thread-safe!"
#endif


/* Base objects and types
 */

typedef const struct ClassTable * Class;
typedef		  struct BaseObject * Object;

/* Object and class manipulation functions
 */

#define		ooc_init_class( pClass ) _ooc_init_class( & pClass ## Class )				/* Initializes the Class */
void		_ooc_init_class( const Class );
#define		ooc_finalize_class( pClass ) _ooc_finalize_class( & pClass ## Class )		/* Finalizes the Class */
void		_ooc_finalize_class( const Class );
void		ooc_finalize_all( void );													/* Finalizes all classes */
#define		ooc_new( pClass, pPar ) ooc_new_classptr( & pClass ## Class, pPar )			/* Creates a new object from class */
Object		ooc_new_classptr( const Class, const void * );	
Object		ooc_duplicate( const Object );				/* Copy constructor */
void		ooc_delete( Object );						/* Deletes an Object */
void		ooc_delete_and_null( Object * );			/* Deletes an Object via a pointer, and NULL the pointer thread safely */

/*	Type checking helpers
 */

#define     ooc_isInitialized( pClass ) _ooc_isInitialized( & pClass ## Class )
#define     ooc_isInstanceOf( pObj, pClass )  _ooc_isInstanceOf( pObj, & pClass ## Class )
Class		ooc_get_type( const Object );
int			_ooc_isInitialized( const Class );
int			_ooc_isInstanceOf( const void * _self, const Class base );

/*	Memory handling
 */

void *		ooc_malloc( size_t size );					/* Allocates memory like malloc(), but throws an Exception on error */
void *		ooc_calloc( size_t num, size_t size );		/* Allocates memory like calloc(), but throws an Exception on error */
void *		ooc_realloc( void *ptr, size_t size );		/* Reallocates memory like realloc(), but throws an Exception on error */
void *		ooc_memdup( const void *ptr, size_t size );	/* duplicates a memory block with a new alloc, throws an Exception on error */
char *		ooc_strdup( const char * s );				/* duplicates a C string with a new alloc, throws an Exception on error */
void 		ooc_free( const void * mem );				/* Frees memory allocated by ooc_malloc(), ooc_calloc() or ooc_realloc() */
void 		ooc_free_and_null( void ** mem );			/* Frees memory via a pointer, and NULL the pointer thread safely */

void * 		ooc_ptr_read_and_null( void ** ptr_ptr );	/* Reads a pointer via a pointer, and nulls, atomic operation */ 

/*	Exception handling
 */

void		ooc_throw( Object );
void		ooc_rethrow( void ); 

struct ooc_try_block {
	Object					exc_obj;
	int						status;
	struct ooc_try_block *	previous;
	jmp_buf					buffer;
	};

#define		try			{ struct ooc_try_block TryContext;		\
						Object exception;						\
						ooc_link_try_block( &TryContext );		\
						if( ! setjmp(TryContext.buffer) )
#define		catch(ec)	else if( (exception = ooc_exception_caught( & ec ## Class ))) 
#define		catch_any	else if( (exception = ooc_exception_caught( NULL )))
#define		finalize	exception = NULL;	/* Only for supressing "unused variable warning" with a pure try .. finalize ..end_try block */
#define		end_try		ooc_end_try(); }

void	ooc_link_try_block( struct ooc_try_block * );
Object	ooc_exception_caught( const Class );
void	ooc_end_try( void );


/* Class components definitions
 */

typedef		  struct BaseVtable_stru * BaseVtable;

typedef              BaseVtable    Vtable;

struct ClassTable
{
	const size_t		size;				/* size of the object */
	const char *		name;				/* the name of the class (for information only) */
	const Class		 	parent;				/* parent of the class */
	Vtable			    vtable;				/* the pointer to the virtual function's table */
	const size_t		vtab_size;			/* the size of the vtable */
	void	(* init) ( Class this );        /* class initializer */
	void	(* finz) ( Class this );		/* class finalizer */
	void	(* ctor) (Object self, const void * params );	/* constructor */
	void	(* dtor) (Object self);			/* destructor */
	int  	(* copy) (Object self, const Object from); /* copy constructor */
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


/*	Class declaration macro
 */

#define	DeclareClass( pClass, pParent )  							\
	typedef struct pClass ## Object * pClass;				\
	extern const struct ClassTable pClass ## Class;

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

#if defined( STIN )	/* Compilers that support function inlining */

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

/*	Class virtual functions declaration macro
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

#define EndOfVirtuals	}


/* Class members declaration macro
 */

#define ClassMembers( pClass, pParent )						\
	struct pClass ## Object {								\
		struct pParent ## Object pParent;				

#define EndOfClassMembers	}

/* Class allocation macro
 */

#define AllocateClass( pClass, pParent )					\
															\
	/* Prototyping */										\
	static void   pClass ## _initialize ( Class );	        \
	static void   pClass ## _finalize ( Class );	        \
	static void   pClass ## _constructor( pClass, const void * ); \
	static void   pClass ## _destructor ( pClass );	        \
	static int	  pClass ## _copy ( pClass, const pClass );	\
															\
	/* Allocating the Vtable */								\
	struct pClass ## Vtable_stru pClass ## VtableInstance;	\
															\
	_define_vtab_access( pClass, pParent );					\
															\
	/* Allocating the class description table */			\
	const struct ClassTable pClass ## Class = {				\
		sizeof( struct pClass ## Object ),					\
		# pClass,											\
		& pParent ## Class,	                                \
		(Vtable) & pClass ## VtableInstance,				\
		sizeof( struct pClass ## Vtable_stru ),				\
		pClass ## _initialize,	                            \
		pClass ## _finalize,	                            \
		(void (*)( Object, const void *)) pClass ## _constructor,	\
		(void (*)( Object))               pClass ## _destructor,	\
		(int  (*)( Object, const Object)) pClass ## _copy	        \
		}
		
/* Parent constructor macro
 */

#define chain_constructor( pClass, pSelf, pParam ) \
	if( pClass ## Class.parent != &BaseClass ) pClass ## Class.parent->ctor( (Object) pSelf, pParam )
	
/*  Function marchaler types
 */

typedef	 void (* void_fn_voidp )( void * );
typedef	 void (* void_fn_voidp_voidp )( void *, void * );
typedef	 int  (* int_fn_voidp_voidp  )( void *, void * );
typedef	 void (* void_fn_Object_voidp)( void *, void * );
typedef	 int  (* int_fn_Object_voidp )( void *, void * );

#endif /* OOC_H */
