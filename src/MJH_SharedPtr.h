/*
 * Copyright (c) 2002, 2004-2009 Matthew J. Healy (mjh@cs.rit.edu)
 * All Rights Reserved Worldwide
 *
 * SharedPtr.h 
 * $Revision: 1.1 $ $Date: 2012/04/18 02:17:54 $
 * 
 * Ob. Legal details may be found below.
 *
 */

#ifndef MJH_SHARED_PTR_H
#define MJH_SHARED_PTR_H

#include <cstdlib>
#include <stdexcept>
#include <functional>   // for std::less specialization
#include <cassert>

/*
 * Compile-time switches
 */

/* Define this symbol to enable lots of output to stdout describing
 * what the class is doing. Note that we're not using the standard 
 * switch "NOISY" here because it's too easy for someone to accidentally
 * define "NOISY" globally and cause this class to import all the 
 * iostream stuff. This causes lots of setup to happen at static
 * initialization time, which can cause problems, particularly when 
 * building for a shared library. 
 */
#undef SHAREDPTR_NOISY

#ifdef SHAREDPTR_NOISY
    #include <iostream>
    #include <iomanip>
#endif


namespace mjh {

/** 
 * This class defines a pointer-checking function that throws a 
 * <code>std::logic_error</code> when a <code>NULL</code> pointer is 
 * detected.  This type is intended for use as a "traits" type for the 
 * shared pointer class, which defines what behavior it should exhibit 
 * when client code tries to "dereference" a shared pointer with a 
 * <code>NULL</code> value.
 */
template <typename T>
struct LogicErrorOnPtrCheckFailure
{
    /**
     * Checks the specified pointer to see if it is valid for use.
     * 
     * @param ptr   the pointer being checked
     * @throw std::logic_error   if the pointer is invalid
     */
    static void checkPointer( T * ptr )
        {
            if ( ptr == NULL ) {
                throw std::logic_error( "No pointer to be dereferenced" );
            }
        }
};

/** 
 * This class defines a pointer-checking function that generates an 
 * assertion when a <code>NULL</code> pointer is detected.  This type 
 * is intended for use as a "traits" type for the shared pointer class, 
 * which defines what behavior it should exhibit when client code tries 
 * to "dereference" a shared pointer with a <code>NULL</code> value.
 * 
 * @note
 *     This code will only detect failures under "debug builds" (where 
 *     assertions are enabled).  Under "release builds" (in which 
 *     assertions are <em>disabled</em>), the failures will go undetected, 
 *     which can yield undefined results.  As a result, client code should 
 *     explicitly check for null values before using the pointers if this 
 *     trait set is in use. 
 */
template <typename T>
struct AssertOnPtrCheckFailure
{
    /**
     * Checks the specified pointer to see if it is valid for use.  If it 
     * is not (i.e., if it is NULL), than we will generate an assertion 
     * failure in debug builds (i.e., when "NDEBUG" is not defined), and do 
     * nothing in release builds.
     *
     * @param ptr   the pointer being checked
     */
    static void checkPointer( T * ptr )
        {
            assert( ptr != NULL );
        }
};

/**
 * A "traits" type used to handle some pieces of a single pointer-referenced 
 * resource allocated with "new".  (In other words, if you're 
 * building a shared pointer to an object allocated with "new SomeType(....)", 
 * then this is the type to specify for the <code>PtrManipTraits</code>
 * aspect of the shared pointer class.)
 * 
 * @note
 *    This class does not define an "arrayAccess" method, since the underlying 
 *    assumption is that we're dealing with a pointer to a single object, 
 *    rather than to an array.
 */
template <typename T>
struct SingleFreeStorePointerManipulator {
    /** 
     * Handles the release of the specified pointer.
     * @param ptr    the pointer to a single object to be released (deleted)
     */
    static void releasePointer( T* & ptr ) {
        delete ptr;
        ptr = 0;
    }
};

/**
 * A "traits" type used to handle some pieces of a pointer-referenced 
 * array allocated with "new".  (In other words, if you're 
 * building a shared pointer to an array allocated with "new SomeType[...]", 
 * then this is the type to specify for the <code>PtrManipTraits</code> 
 * aspect of the shared pointer class.)
 */
template <typename T>
struct ArrayFreeStorePointerManipulator {
    /** 
     * Handles the release of the specified array pointer.
     * @param ptr    the pointer to an array to be released (deleted)
     */
    static void releasePointer( T* & ptr ) {
        delete [] ptr;
        ptr = 0;
    }
    
    /** 
     * Returns the specified element of the pointed-to array.
     * 
     * @warning
     *   This function does not perform any validation of the specified 
     *   array index.  If an invalid index is specified, the results of 
     *   this function are undefined.   
     * 
     * @param ptr    the pointer to the array
     * @param index  the index of the desired element in the array
     * @return the indexed element in the array
     */
    static T & arrayAccess( T* const & ptr, int index ) {
        return ptr[index];
    }
};

/**
 * This class provides an implementation of a shared pointer (with 
 * reference counting) to objects.  This allows pointers to be easily 
 * (and safely) stored within an STL container class, or to be passed 
 * around as reference-counted object.
 * <p>
 *    In other words, this class gives you something that can be used 
 *    a lot like Java "object references" are.  When the last "smart 
 *    pointer" that knows about a dynamically allocated object goes 
 *    out of scope, it will destroy that object for you, just like Java 
 *    would schedule stuff for garbage collection that was no longer 
 *    reachable.  The analogy isn't complete -- you can still have 
 *    dangling "raw pointers" to the object, which can cause you 
 *    trouble -- but it does help to improve things as you begin to 
 *    assume the responsibility for memory management that the JVM 
 *    used to handle for you.
 * </p>
 *
 * @note
 *    Care must be used if this class is being used as a wrapper for 
 *    dynamically allocated \em arrays, as an appropriate type must be 
 *    specified for the <code>PtrManipTraits</code> template parameter in 
 *    order to allow array element \em access as well as correct 
 *    \em deletion of the array.  (It may be more desirable/simpler to use a 
 *    class like std::vector for that functionality, if you want it.)
 *
 * @attention
 *    This class is \em not intended to provide thread-safe operations.  If
 *    this type of support is needed, it must be provided by a higher-level 
 *    layer within an application.
 * 
 * @param Type          the type of object that the shared pointer will 
 *                      refer to
 * @param PtrFailureTraits   a traits type defining the behavior of 
 *                           the shared pointer when client code tries 
 *                           to dereference a "null" pointer
 * @param PtrManipTraits     a traits type used to define how the underlying 
 *                           pointer should be manipulated (i.e., how it 
 *                           is deleted/released, and optionally providing 
 *                           array indexing support)
 * 
 * @version $Id: MJH_SharedPtr.h,v 1.1 2012/04/18 02:17:54 hlh3364 Exp hlh3364 $
 * @author  <a href="mailto:mjh@cs.rit.edu">Matt Healy</a>
 */
template <
        typename Type, 
        typename PtrFailureTraits = AssertOnPtrCheckFailure<Type>, 
        typename PtrManipTraits = SingleFreeStorePointerManipulator<Type>
    >
class SharedPtr {
    public:
        /** Constructs a SharedPtr object referring to <code>NULL</code>. */
        SharedPtr();

        /** Releases any resources used by the SharedPtr object.
          *
          * @see #release
          */
        ~SharedPtr();

        /** Constructs a new SharedPtr that assumes ownership of the 
          * specified object.  
          *
          * @attention
          *    You should <i>never</i> create two SharedPtr objects
          *    directly from the same <i>raw</i> pointer.  If you do, they
          *    will not know that the underlying object is being shared,
          *    which will lead to double-deletion of the data.  If you want
          *    more than one SharedPtr to refer to a "naked object", then
          *    you should create one with this constructor, and then create
          *    the other SharedPtr objects via the copy-constructor or
          *    copy-assignment.
          */
        explicit SharedPtr( Type * rawPtr );

        /** Constructs a new SharedPtr that will share ownership of the 
          * data referred to by the src object.
          */
        SharedPtr( const SharedPtr & src );

        /**
         * Allow a shared pointer to be converted to a shared pointer to 
         * a compatible type (e.g., "ptrToConst = ptrToMutable", or 
         * "ptrToBase = ptrToDerived").
         */
        template<typename Type2, typename Traits2, typename ManipTraits2> 
        SharedPtr( const SharedPtr<Type2, Traits2, ManipTraits2> & src );

        /** Updates the SharedPtr object so that it shares the data referred to 
          * by the src object.  (If the SharedPtr object was the only one 
          * referring to an underlying ("wrapped") object, that object will 
          * be released.)
          */
        SharedPtr & operator=( const SharedPtr & src );

        /** Swaps this object's data with that of <code>other</code>.
          * (When used correctly, this can help improve exception-safety, 
          * and is frequently used by the STL for exactly that purpose.)
          */
        void swap( SharedPtr & other );

        /** Compares the current object to the other SharedPtr, returning 
          * true if they both wrap the same object.
          *
          * @attention
          *    This does <i>not</i> compare the wrapped objects to see if 
          *    they are equivalent: it compares <b>identity</b>, not value.  
          *    In order to perform a value-based test, you should evaluate 
          *    <code>( *aSharedPtr == *other )</code>, after first ensuring 
          *    that neither SharedPtr contains <code>NULL</code>.
          */
        bool operator==( const SharedPtr & other ) const;

        /** Compares the current object to the other SharedPtr, returning 
          * false if they both wrap the same object.
          *
          * @attention
          *    This does <i>not</i> compare the wrapped objects to see if 
          *    they are equivalent: it compares <b>identity</b>, not value.  
          *    In order to perform a value-based test, you should evaluate 
          *    <code>( *aSharedPtr != *other )</code>, after first ensuring 
          *    that neither SharedPtr contains <code>NULL</code>.
          */
        bool operator!=( const SharedPtr & other ) const;

        /** Resets the SharedPtr so that it no longer refers to any object. 
          * (This will result in the wrapped object being released, 
          * if no other SharedPtrs refer to it.)
          */
        void release();

        /** Releases the resource held by the object and stores a reference to
          * the provided object instead.
          */
        void reset( Type * rawPtr = NULL );

        /** Updates the SharedPtr so that it has its own (unshared) copy 
          * of its wrapped object.  
          *
          * <p>
          *    This is intended to enable "copy on write" semantics, where 
          *    any number of objects may have references to a common (shared) 
          *    object until one of them needs to change its own copy.  In 
          *    such a case, that object should first call %makeUnique() on its 
          *    own SharedPtr reference, so that subsequent changes applied 
          *    through the (now-uniqued) SharedPtr will not affect the original 
          *    object that anyone else still refers to.
          * </p>
          */
        void makeUnique();

        /** Allows the "->" operator to be used with the SharedPtr object as 
          * if it was a "normal" pointer to the wrapped entity.
          */
        Type * operator->() const;

        /** Allows the "*" operator to be used with the SharedPtr object as 
          * if it was a "normal" pointer to the wrapped entity.
          */
        Type & operator*() const;

        /** Returns a "raw" pointer to the entity owned by this object. 
          *
          * <p>
          *    This function should be used with <strong>extreme</strong> 
          *    caution, since the SharedPtr object will still "own" the 
          *    object that the return result points to.  This means that if 
          *    the SharedPtr object changes, it could release the underlying 
          *    object, which would result in the caller being left with a 
          *    dangling pointer (i.e., a crash/failure just waiting to 
          *    happen).
          * </p>
          */
        Type * get() const;

        /** Convenience function, returning the reference count for the 
          * shared object.
          */
        int getUsageCount() const {
            return (refCount != NULL ? *refCount : 0);
        }

        /**
         * Returns the specified element of a shared array.
         * 
         * @note
         *   This function is only accessible if the 
         *   <code>PtrManipTraits</code> template parameter provides an 
         *   appropriate <code>arrayAccess</code> method.
         * 
         * @param index  the index of the desired element in the shared array
         * 
         * @return the indexed element in the array
         * 
         * @see ArrayFreeStorePointerManipulator
         */
        Type & operator[]( int index ) const;

    private:

        /** 
         * Increments the reference count for the object.  This is a 
         * convenience method, intended to help prevent mis-use of 
         * the reference count pointer.
         */
        void incrementCount() {
            if ( NULL != refCount ) {
                *refCount += 1;
            }
        }

        /** 
         * Decrements the reference count for the object.  This is a 
         * convenience method, intended to help prevent mis-use of 
         * the reference count pointer.
         */
        void decrementCount() {
            if ( NULL != refCount ) {
                *refCount -= 1;
            }
        }

    private:

        /** A raw pointer to the object owned by a SharedPtr */
        Type * ptr;

        /** The reference count for the object owned by a SharedPtr */
        volatile int * refCount;

        /** 
         * Used to enable conversions between shared pointers to compatible 
         * types. 
         */
        template <typename Type2, typename Traits2, typename ManipTraits2> 
        friend class SharedPtr;

};  // class SharedPtr

}   // namespace "mjh"


//////////////////////////////////////////////////////////////////////////////
//  specializations for things in namespace std  -----------------------------//
//

#if !defined( _MSC_VER ) || (_MSC_VER > 1200)   // MSVC6 chokes on the following
namespace std {
/**
 * Specializes the std::less function object ("functor"), so that we can 
 * use shared pointers as keys in associative collections.  
 *
 * @attention
 *    When this functor is used, the shared pointers will be evaluated in 
 *    exactly the same way as the underlying (raw) pointer values: the 
 *    "natural ordering" of the memory addresses will be used, rather than 
 *    any "natural ordering" of the referenced data.  (In other words, this 
 *    will compare object <em>identities</em>, rather than object values.)
 *
 * @note
 *    It's debatable whether this is better than supplying a full range of 
 *    comparison operators (<, >, <=, >=), but I'd like to keep the risk of 
 *    confusion to a minimum, and this provides an example for providing 
 *    the "functor" definitions for the other kinds of comparisons.
 */
template<typename T, typename C>
struct less< mjh::SharedPtr<T, C> >
    : binary_function<mjh::SharedPtr<T, C>, mjh::SharedPtr<T, C>, bool>
{
    bool operator()(const mjh::SharedPtr<T, C>& a,
                    const mjh::SharedPtr<T, C>& b) const {
        return less<T*>()( a.get(), b.get() );
    }
};
}   // namespace "std" (specializations)
#endif

//////////////////////////////////////////////////////////////////////////////
// SharedPtr<typename Type> method definitions
// (documentation is provided above)
//

template<typename Type, typename PtrFailureTraits, typename PtrManipTraits>
mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits>::SharedPtr()
        : ptr( NULL ), refCount( NULL )
{
}

template<typename Type, typename PtrFailureTraits, typename PtrManipTraits>
mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits>::~SharedPtr()
{
    release();
}

template<typename Type, typename PtrFailureTraits, typename PtrManipTraits>
mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits>::SharedPtr( Type * rawPtr ) 
{
    if ( NULL == rawPtr ) {
        ptr = NULL;
        refCount = NULL;
    }
    else {
        try {
            refCount = new int(0);
        }
        catch( ... ) {
            PtrManipTraits::releasePointer( rawPtr );
            throw;
        }

        // OK, make the changes
        incrementCount();
        ptr = rawPtr;

#ifdef SHAREDPTR_NOISY
        std::cout << "Adopting data at: 0x" 
                  << std::setfill('0') << std::setw(8) << int(ptr)
                  << std::endl;
#endif  // SHAREDPTR_NOISY
    }
}

template<typename Type, typename PtrFailureTraits, typename PtrManipTraits>
mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits>::SharedPtr( 
        const mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits> & src 
    ) 
    : ptr( src.ptr ), refCount( src.refCount )
{
    if ( NULL != refCount ) {
        incrementCount();

#ifdef SHAREDPTR_NOISY
        std::cout << "Sharing data at: 0x" 
                  << std::setfill('0') << std::setw(8) << int(ptr)
                  << std::endl;
#endif  // SHAREDPTR_NOISY
    }
}

template< typename Type, typename PtrFailureTraits, typename PtrManipTraits >
template < typename Type2, typename Traits2, typename ManipTraits2 > 
mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits>::SharedPtr( 
        const SharedPtr<Type2, Traits2, ManipTraits2> & src 
    )
    : ptr( src.ptr ), refCount( src.refCount )
{
    if ( NULL != refCount ) {
        incrementCount();

#ifdef SHAREDPTR_NOISY
        std::cout << "Sharing data at: 0x" 
                  << std::setfill('0') << std::setw(8) << int(ptr)
                  << std::endl;
#endif  // SHAREDPTR_NOISY
    }
}

template<typename Type, typename PtrFailureTraits, typename PtrManipTraits>
mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits> & 
mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits>::operator=( 
        const mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits> & src 
    ) 
{
    SharedPtr<Type> temp( src );
    swap( temp );
    return *this;
}

template<typename Type, typename PtrFailureTraits, typename PtrManipTraits>
void mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits>::swap( 
        mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits> & other 
    ) 
{
    std::swap( ptr, other.ptr );
    std::swap( refCount, other.refCount );
}

template<typename Type, typename PtrFailureTraits, typename PtrManipTraits>
bool mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits>::operator==( 
        const mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits> & other 
    ) const 
{
    bool result = (ptr == other.ptr);
    return result;
}

template<typename Type, typename PtrFailureTraits, typename PtrManipTraits>
bool mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits>::operator!=( 
        const mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits> & other 
    ) const 
{
    return !(*this == other);
}


template<typename Type, typename PtrFailureTraits, typename PtrManipTraits>
void mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits>::release() {
    if ( NULL != refCount ) {
#ifdef SHAREDPTR_NOISY
        std::cout << "Releasing data at: 0x" 
                  << std::setfill('0') << std::setw(8) << int(ptr)
                  << std::endl;
#endif

        decrementCount();

        if ( 0 == *refCount ) {
#ifdef SHAREDPTR_NOISY
            std::cout << "\tDeleting data at: 0x" 
                      << std::setfill('0') << std::setw(8) << int(ptr)
                      << std::endl;
#endif
            PtrManipTraits::releasePointer( ptr );
            delete refCount;
        }

        // One way or another, we no longer share anything....
        ptr = NULL;
        refCount = NULL;
    }
}

template<typename Type, typename PtrFailureTraits, typename PtrManipTraits>
void mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits>::reset( Type * rawPtr ) {
    *this = mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits>( rawPtr );
}

template<typename Type, typename PtrFailureTraits, typename PtrManipTraits>
void mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits>::makeUnique() {
    assert( ptr == NULL || refCount != NULL );

    if ( NULL != refCount && (*refCount > 1) ) {
        if ( NULL != ptr ) {
            SharedPtr< Type, PtrFailureTraits > temp( new Type( *ptr ) );
            swap( temp );

#ifdef SHAREDPTR_NOISY
            std::cout << "Unique data now at: 0x" 
                      << std::setfill('0') << std::setw(8) << int(ptr)
                      << std::endl;
#endif
        }
    }
}

template<typename Type, typename PtrFailureTraits, typename PtrManipTraits>
Type * mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits>::operator->() const {
    PtrFailureTraits::checkPointer( ptr );
    return ptr;
}

template<typename Type, typename PtrFailureTraits, typename PtrManipTraits>
Type & mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits>::operator*() const {
    PtrFailureTraits::checkPointer( ptr );
    return *ptr;
}

template<typename Type, typename PtrFailureTraits, typename PtrManipTraits>
Type & mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits>::operator[]( int index ) const {
    PtrFailureTraits::checkPointer( ptr );
    return PtrManipTraits::arrayAccess( ptr, index );
}


template<typename Type, typename PtrFailureTraits, typename PtrManipTraits>
Type * mjh::SharedPtr<Type, PtrFailureTraits, PtrManipTraits>::get() const {
    return ptr;
}

/**
 * @example SharedPtrSample.cpp 
 * This is an example of some code that uses the mjh::SharedPtr type to safely 
 * manage dynamically allocated objects that are being stored in an STL 
 * container (i.e., a std::vector).  As examples go, it's a \em trifle 
 * contrived, but it gets the job done....
 */

/**
 * @example SharedArraySample.cpp 
 * This is an example of some code that uses the mjh::SharedPtr type to safely 
 * manage dynamically allocated arrays of objects.  
 * 
 * @note This functionality is still somewhat experimental, and should be 
 *       treated as such.
 */

/**
 * @example TestSharedPtr.cpp 
 * This is a unit test driver that I put together for the mjh::SharedPtr 
 * class.
 */

/*
 * Ob. Legal details:
 *     Permission is hereby granted, free of charge, to any person obtaining
 *     a copy of this software and associated documentation files (the
 *     ``Software''), to deal in the Software without restriction, including
 *     without limitation the rights to use, copy, modify, merge, publish,
 *     distribute, sublicense, and/or sell copies of the Software, and to
 *     permit persons to whom the Software is furnished to do so, subject to
 *     the following conditions:
 *
 *     The copyright notice above and this permission notice shall be included
 *     in all copies or substantial portions of the Software.
 *
 *     THE SOFTWARE IS PROVIDED ``AS IS'', WITHOUT WARRANTY OF ANY KIND, 
 *     EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *     MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *     IN NO EVENT SHALL MATTHEW J. HEALY BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *     OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *     ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *     OTHER DEALINGS IN THE SOFTWARE.
 *
 *     Except as contained in this notice, the name of Matthew J. Healy
 *     shall not be used in advertising or otherwise to promote the sale, 
 *     use or other dealings in this Software without prior written 
 *     authorization from Matthew J. Healy.
 */

#endif  // MJH_SHARED_PTR_H

