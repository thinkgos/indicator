
#ifndef __LIST_H__
#define __LIST_H__

#include "common_type.h"

/*
 * The list structure members are modified from within interrupts, and therefore
 * by rights should be declared volatile.  However, they are only modified in a
 * functionally atomic way (within critical sections of with the scheduler
 * suspended) and are either passed by reference into a function or indexed via
 * a volatile variable.  Therefore, in all use cases tested so far, the volatile
 * qualifier can be omitted in order to provide a moderate performance
 * improvement without adversely affecting functional behaviour.  The assembly
 * instructions generated by the IAR, ARM and GCC compilers when the respective
 * compiler's options were set for maximum optimisation has been inspected and
 * deemed to be as intended.  That said, as compiler technology advances, and
 * especially if aggressive cross module optimisation is used (a use case that
 * has not been exercised to any great extend) then it is feasible that the
 * volatile qualifier will be needed for correct optimisation.  It is expected
 * that a compiler removing essential code because, without the volatile
 * qualifier on the list structure members and with aggressive cross module
 * optimisation, the compiler deemed the code unnecessary will result in
 * complete and obvious failure of the scheduler.  If this is ever experienced
 * then the volatile qualifier can be inserted in the relevant places within the
 * list structures by simply defining configLIST_VOLATILE to volatile in
 * FreeRTOSConfig.h (as per the example at the bottom of this comment block).
 * If configLIST_VOLATILE is not defined then the preprocessor directives below
 * will simply #define configLIST_VOLATILE away completely.
 *
 * To use volatile list structure members then add the following line to
 * FreeRTOSConfig.h (without the quotes):
 * "#define configLIST_VOLATILE volatile"
 */
 
#define configLIST_VOLATILE
#define LIST_ITEM_VALUE_MAX  ( uint32_t )0xffffffffUL

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Definition of the only type of object that a list can contain.
 */
struct ListItem_s
{
    configLIST_VOLATILE uint32_t xItemValue;          /*< The value being listed.  In most cases this is used to sort the list in descending order. */
    struct ListItem_s * configLIST_VOLATILE pxNext;     /*< Pointer to the next ListItem_t in the list. */
    struct ListItem_s * configLIST_VOLATILE pxPrevious; /*< Pointer to the previous ListItem_t in the list. */
    void * pvOwner;                                     /*< Pointer to the object (normally a TCB) that contains the list item.  There is therefore a two way link between the object containing the list item and the list item itself. */
    void * configLIST_VOLATILE pvContainer;             /*< Pointer to the list in which this list item is placed (if any). */
};
typedef struct ListItem_s ListItem_t;                   /* For some reason lint wants this as two separate definitions. */

struct MiniListItem_s
{
    configLIST_VOLATILE uint32_t xItemValue;
    struct ListItem_s * configLIST_VOLATILE pxNext;
    struct ListItem_s * configLIST_VOLATILE pxPrevious;
};
typedef struct MiniListItem_s MiniListItem_t;
/*
 * Definition of the type of queue used by the scheduler.
 */
typedef struct List_s
{
    configLIST_VOLATILE uint32_t uxNumberOfItems;
    ListItem_t * configLIST_VOLATILE pxIndex;           /*< Used to walk through the list.  Points to the last item returned by a call to listGET_OWNER_OF_NEXT_ENTRY (). */
    MiniListItem_t xListEnd;                            /*< List item that contains the maximum possible item value meaning it is always at the end of the list and is therefore used as a marker. */
} List_t;

//静态结构体,用于屏蔽用户对结构体的可见
typedef struct ListItemStatic_s
{
	uint32_t xDummy0;
	void *pvDummy1[ 4 ];
}ListItemStatic_t;

typedef struct MiniListItemStatic_s
{
    uint32_t xDummy0;
    void *pvDummy1[ 2 ];
}MiniListItemStatic_t;

typedef struct ListStatic_s
{
    uint32_t xDummy0;
    void *pvDummy1;
    MiniListItemStatic_t xDummy2;
}ListStatic_t;

/*
 * Access macro to set the owner of a list item.  The owner of a list item
 * is the object (usually a TCB) that contains the list item.
 *
 * \page listSET_LIST_ITEM_OWNER listSET_LIST_ITEM_OWNER
 * \ingroup LinkedList
 */
#define listSET_LIST_ITEM_OWNER( pxListItem, pxOwner )      ( ( pxListItem )->pvOwner = ( void * ) ( pxOwner ) )

/*
 * Access macro to get the owner of a list item.  The owner of a list item
 * is the object (usually a TCB) that contains the list item.
 *
 * \page listSET_LIST_ITEM_OWNER listSET_LIST_ITEM_OWNER
 * \ingroup LinkedList
 */
#define listGET_LIST_ITEM_OWNER( pxListItem )   ( ( pxListItem )->pvOwner )

/*
 * Access macro to set the value of the list item.  In most cases the value is
 * used to sort the list in descending order.
 *
 * \page listSET_LIST_ITEM_VALUE listSET_LIST_ITEM_VALUE
 * \ingroup LinkedList
 */
#define listSET_LIST_ITEM_VALUE( pxListItem, xValue )   ( ( pxListItem )->xItemValue = ( xValue ) )

/*
 * Access macro to retrieve the value of the list item.  The value can
 * represent anything - for example the priority of a task, or the time at
 * which a task should be unblocked.
 *
 * \page listGET_LIST_ITEM_VALUE listGET_LIST_ITEM_VALUE
 * \ingroup LinkedList
 */
#define listGET_LIST_ITEM_VALUE( pxListItem )   ( ( pxListItem )->xItemValue )

/*
 * Access macro to retrieve the value of the list item at the head of a given
 * list.
 *
 * \page listGET_LIST_ITEM_VALUE listGET_LIST_ITEM_VALUE
 * \ingroup LinkedList
 */
#define listGET_ITEM_VALUE_OF_HEAD_ENTRY( pxList )  ( ( ( pxList )->xListEnd ).pxNext->xItemValue )

/*
 * Return the list item at the head of the list.
 *
 * \page listGET_HEAD_ENTRY listGET_HEAD_ENTRY
 * \ingroup LinkedList
 */
#define listGET_HEAD_ENTRY( pxList )    ( ( ( pxList )->xListEnd ).pxNext )

/*
 * Return the list item at the head of the list.
 *
 * \page listGET_NEXT listGET_NEXT
 * \ingroup LinkedList
 */
#define listGET_NEXT( pxListItem )  ( ( pxListItem )->pxNext )
// by mo
#define listGET_NEXT_ENTRY( pxListItem ) listGET_NEXT( pxListItem )
/*
 * Return the list item that marks the end of the list
 *
 * \page listGET_END_MARKER listGET_END_MARKER
 * \ingroup LinkedList
 */
#define listGET_END_MARKER( pxList )    ( ( ListItem_t const * ) ( &( ( pxList )->xListEnd ) ) )

/*
 * Access macro to determine if a list contains any items.  The macro will
 * only have the value true if the list is empty.
 *
 * \page listLIST_IS_EMPTY listLIST_IS_EMPTY
 * \ingroup LinkedList
 */
#define listLIST_IS_EMPTY( pxList ) ( ( uint8_t ) ( ( pxList )->uxNumberOfItems == ( uint32_t ) 0 ) )

/*
 * Access macro to return the number of items in the list.
 */
#define listCURRENT_LIST_LENGTH( pxList )   ( ( pxList )->uxNumberOfItems )

/*
 * Access function to obtain the owner of the next entry in a list.
 *
 * The list member pxIndex is used to walk through a list.  Calling
 * listGET_OWNER_OF_NEXT_ENTRY increments pxIndex to the next item in the list
 * and returns that entry's pxOwner parameter.  Using multiple calls to this
 * function it is therefore possible to move through every item contained in
 * a list.
 *
 * The pxOwner parameter of a list item is a pointer to the object that owns
 * the list item.  In the scheduler this is normally a task control block.
 * The pxOwner parameter effectively creates a two way link between the list
 * item and its owner.
 *
 * @param pxTCB pxTCB is set to the address of the owner of the next list item.
 * @param pxList The list from which the next item owner is to be returned.
 *
 * \page listGET_OWNER_OF_NEXT_ENTRY listGET_OWNER_OF_NEXT_ENTRY
 * \ingroup LinkedList
 */
#define listGET_OWNER_OF_NEXT_ENTRY( pxTCB, pxList )                                        \
{                                                                                           \
List_t * const pxConstList = ( pxList );                                                    \
    /* Increment the index to the next item and return the item, ensuring */                \
    /* we don't return the marker used at the end of the list.  */                          \
    ( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxNext;                            \
    if( ( void * ) ( pxConstList )->pxIndex == ( void * ) &( ( pxConstList )->xListEnd ) )  \
    {                                                                                       \
        ( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxNext;                        \
    }                                                                                       \
    ( pxTCB ) = ( pxConstList )->pxIndex->pvOwner;                                          \
}


/*
 * Access function to obtain the owner of the first entry in a list.  Lists
 * are normally sorted in ascending item value order.
 *
 * This function returns the pxOwner member of the first item in the list.
 * The pxOwner parameter of a list item is a pointer to the object that owns
 * the list item.  In the scheduler this is normally a task control block.
 * The pxOwner parameter effectively creates a two way link between the list
 * item and its owner.
 *
 * @param pxList The list from which the owner of the head item is to be
 * returned.
 *
 * \page listGET_OWNER_OF_HEAD_ENTRY listGET_OWNER_OF_HEAD_ENTRY
 * \ingroup LinkedList
 */
#define listGET_OWNER_OF_HEAD_ENTRY( pxList )  ( (&( ( pxList )->xListEnd ))->pxNext->pvOwner )

/*
 * Check to see if a list item is within a list.  The list item maintains a
 * "container" pointer that points to the list it is in.  All this macro does
 * is check to see if the container and the list match.
 *
 * @param pxList The list we want to know if the list item is within.
 * @param pxListItem The list item we want to know if is in the list.
 * @return pdTRUE if the list item is in the list, otherwise pdFALSE.
 */
#define listIS_CONTAINED_WITHIN( pxList, pxListItem ) ( ( uint8_t ) ( ( pxListItem )->pvContainer == ( void * ) ( pxList ) ) )

/*
 * Return the list a list item is contained within (referenced from).
 *
 * @param pxListItem The list item being queried.
 * @return A pointer to the List_t object that references the pxListItem
 */
#define listLIST_ITEM_CONTAINER( pxListItem ) ( ( pxListItem )->pvContainer )

/*
 * This provides a crude means of knowing if a list has been initialised, as
 * pxList->xListEnd.xItemValue is set to LIST_ITEM_VALUE_MAX by the listInitialise()
 * function.
 */
#define listLIST_IS_INITIALISED( pxList ) ( ( pxList )->xListEnd.xItemValue == LIST_ITEM_VALUE_MAX )

/*
 * Must be called before a list is used!  This initialises all the members
 * of the list structure and inserts the xListEnd item into the list as a
 * marker to the back of the list.
 *
 * @param pxList Pointer to the list being initialised.
 *
 * \page listInitialise listInitialise
 * \ingroup LinkedList
 */
void listInitialise( List_t * const pxList );

/*
 * Must be called before a list item is used.  This sets the list container to
 * null so the item does not think that it is already contained in a list.
 *
 * @param pxItem Pointer to the list item being initialised.
 *
 * \page listInitialiseItem listInitialiseItem
 * \ingroup LinkedList
 */
void listInitialiseItem( ListItem_t * const pxItem );

/*
 * Insert a list item into a list.  The item will be inserted into the list in
 * a position determined by its item value (descending item value order).
 *
 * @param pxList The list into which the item is to be inserted.
 *
 * @param pxNewListItem The item that is to be placed in the list.
 *
 * \page listInsert listInsert
 * \ingroup LinkedList
 */
void listInsert( List_t * const pxList, ListItem_t * const pxNewListItem );

/*
 * Insert a list item into a list.  The item will be inserted in a position
 * such that it will be the last item within the list returned by multiple
 * calls to listGET_OWNER_OF_NEXT_ENTRY.
 *
 * The list member pxIndex is used to walk through a list.  Calling
 * listGET_OWNER_OF_NEXT_ENTRY increments pxIndex to the next item in the list.
 * Placing an item in a list using listInsertEnd effectively places the item
 * in the list position pointed to by pxIndex.  This means that every other
 * item within the list will be returned by listGET_OWNER_OF_NEXT_ENTRY before
 * the pxIndex parameter again points to the item being inserted.
 *
 * @param pxList The list into which the item is to be inserted.
 *
 * @param pxNewListItem The list item to be inserted into the list.
 *
 * \page listInsertEnd listInsertEnd
 * \ingroup LinkedList
 */
void listInsertEnd( List_t * const pxList, ListItem_t * const pxNewListItem );

/*
 * Remove an item from a list.  The list item has a pointer to the list that
 * it is in, so only the list item need be passed into the function.
 *
 * @param listRemove The item to be removed.  The item will remove itself from
 * the list pointed to by it's pxContainer parameter.
 *
 * @return The number of items that remain in the list after the list item has
 * been removed.
 *
 * \page listRemove listRemove
 * \ingroup LinkedList
 */
uint32_t listRemove( ListItem_t * const pxItemToRemove );

#ifdef __cplusplus
}
#endif

#endif

