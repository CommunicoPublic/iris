/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _AVL_TREE_HPP__
#define _AVL_TREE_HPP__ 1

/**
  @file AVLTree.hpp
  @brief AVL tree implementation
*/
#include "STLFunctional.hpp"
#include "MallocAllocator.hpp"
#include "STLSstream.hpp"
#include <stdio.h>

namespace IRIS
{
/**
  @struct AVLTreeNode AVLTree.hpp <AVLTree.hpp>
  @brief AVL tree node
*/
template <typename T>struct AVLTreeNode
{
	/** Key               */
	T                   key;
	/** Left subtree      */
	AVLTreeNode<T>    * left;
	/** Right subtree     */
	AVLTreeNode<T>    * right;
	/** Balance factor    */
	CHAR_8              bf;
};

/**
  @class AVLTree AVLTree.hpp <AVLTree.hpp>
  @brief AVL tree implementation
*/
template <typename T, typename CMP = STLW::less<T>, typename DataAllocator = MallocAllocator>class AVLTree
{
public:
	/**
	  @brief Constructor
	  @param oAllocator - fixed size data allocator
	*/
	AVLTree(CMP            * pICMP = NULL,
	        DataAllocator  * pIAllocator = NULL);

	/**
	  @brief Insert key into tree
	  @param oKey - key to insert
	  @return true, if success, false - if element already exist in tree
	*/
	T * Insert(const T & oKey);

	/**
	  @brief Remove key from tree
	  @param oKey - key to remove
	  @return element value, if success, or T(), if element does not exist
	*/
	T Remove(const T  & oKey);

	/**
	  @brief Remove key from tree
	  @param oKey - key to remove
	  @return element value, if success, or T(), if element does not exist
	*/
	T Remove(const T  & oKey,
	         bool     & bRemoved);

	/**
	  @brief Search key
	  @param oKey - key to search
	  @return Pointer to key, if found, NULL - if element does not exist
	*/
	T * Find(const T  & oKey);

	/**
	  @brief Search least upper bound element
	  @param oKey - key to search
	  @return Pointer to key, if found, NULL - if element does not exist
	*/
	T * Supremum(const T  & oKey);

	/**
	  @brief Search least upper bound element
	  @param oKey - key to search
	  @return Pointer to key, if found, NULL - if element does not exist
	*/
	T * Infimum(const T  & oKey);

	/**
	  @brief Search least upper bound element
	  @param oKey - key to search
	  @return Pointer to key, if found, NULL - if element does not exist
	*/
	T * UpperBound(const T  & oKey);

	/**
	  @brief Search least upper bound element
	  @param oKey - key to search
	  @return Pointer to key, if found, NULL - if element does not exist
	*/
	T * LowerBound(const T  & oKey);

	/**
	  @brief Get number of elements
	*/
	UINT_64 Size();

	/**
	  @brief Check tree is empty
	*/
	bool Empty();

	/**
	  @brief Destructor
	*/
	~AVLTree() throw();

	/**
	  @brief Get tree node size
	*/
	static UINT_64 NodeSize();

	/**
	  @brief Print tree
	*/
	void Print(STLW::stringstream  & oStream);
private:
	// Does not exist
	AVLTree(const AVLTree  & oRhs);
	AVLTree& operator=(const AVLTree  & oRhs);

	/** Left subtree is heavy  */
	static const CHAR_8 C_AVL_LEFT_HEAVY;
	/** Subtree is balanced    */
	static const CHAR_8 C_AVL_BALANCED;
	/** Right subtree is heavy */
	static const CHAR_8 C_AVL_RIGHT_HEAVY;

	/** Memory allocator    */
	CMP               * pCMP;
	/** Memory allocator    */
	DataAllocator     * pDataAllocator;
	/** Tree root           */
	AVLTreeNode<T>    * pRootNode;
	/** Number of elements  */
	UINT_64             iSize;

	/**
	  @brief Search element in tree
	  @param oKey - key to search
	  @param pNode - Subtree
	*/
	AVLTreeNode<T> * SupremumR(const T         & oKey,
	                           AVLTreeNode<T>  * pNode);

	/**
	  @brief Search element in tree
	  @param oKey - key to search
	  @param pNode - Subtree
	*/
	AVLTreeNode<T> * InfimumR(const T         & oKey,
	                          AVLTreeNode<T>  * pNode);

	/**
	  @brief Search element in tree
	  @param oKey - key to search
	  @param pNode - Subtree
	*/
	AVLTreeNode<T> * LowerBoundR(const T         & oKey,
	                             AVLTreeNode<T>  * pNode);

	/**
	  @brief Search element in tree
	  @param oKey - key to search
	  @param pNode - Subtree
	*/
	AVLTreeNode<T> * UpperBoundR(const T         & oKey,
	                             AVLTreeNode<T>  * pNode);

	/**
	  @brief Rotate node left
	  @param pNode - Node to rotate
	*/
	CHAR_8 RotateLeft(AVLTreeNode<T> * & pNode);

	/**
	  @brief Rotate node right
	  @param pNode - Node to rotate
	*/
	CHAR_8 RotateRight(AVLTreeNode<T> * & pNode);

	/**
	  @brief Rotate node left, twice
	  @param pNode - Node to rotate
	*/
	CHAR_8 DblRotateLeft(AVLTreeNode<T> * & pNode);

	/**
	  @brief Rotate node right, twice
	  @param pNode - Node to rotate
	*/
	CHAR_8 DblRotateRight(AVLTreeNode<T> * & pNode);

	/**
	  @brief Rebalance subtree
	  @param pNode - Subtree
	  @return Subtree balance factor
	*/
	CHAR_8 Rebalance(AVLTreeNode<T> * & pNode);

	/**
	  @breif Remove element from tree
	*/
	T  RemoveRec(const T           & oKey,
	             AVLTreeNode<T>  * & pNode,
	             CHAR_8            & iChangeFlag,
	             bool              & bRemoved,
	             CHAR_8              iCompareType = 0);

	/**
	  @brief Insert element into tree
	*/
	AVLTreeNode<T> * InsertRec(const T          & oKey,
	                           AVLTreeNode<T> * & pTree,
	                           CHAR_8           & iNewBF);

	/**
	  @brief Print tree
	*/
	void Print(STLW::stringstream  & oStream,
	           AVLTreeNode<T>      * pNode,
	           const UINT_32         iLevel);

	/**
	  @brief Free used memory
	*/
	void FreeRec(AVLTreeNode<T>  * pNode);
};

template <class T, typename CMP, typename DataAllocator> const CHAR_8 AVLTree<T, CMP, DataAllocator>::C_AVL_LEFT_HEAVY  = -1;
template <class T, typename CMP, typename DataAllocator> const CHAR_8 AVLTree<T, CMP, DataAllocator>::C_AVL_BALANCED    = 0;
template <class T, typename CMP, typename DataAllocator> const CHAR_8 AVLTree<T, CMP, DataAllocator>::C_AVL_RIGHT_HEAVY = 1;

//
// Get tree node size
//
template <class T, typename CMP, typename DataAllocator> UINT_64 AVLTree<T, CMP, DataAllocator>::NodeSize() { return sizeof(AVLTreeNode<T>); }


//
// Check tree is empty
//
template <class T, typename CMP, typename DataAllocator> bool AVLTree<T, CMP, DataAllocator>::Empty() { return Size() == 0; }


//
// Constructor
//
template <class T, typename CMP, typename DataAllocator>AVLTree<T, CMP, DataAllocator>::AVLTree(CMP            * pICMP,
                                                                                                DataAllocator  * pIDataAllocator): pCMP(pICMP),
                                                                                                                                   pDataAllocator(pIDataAllocator),
                                                                                                                                   pRootNode(NULL),
                                                                                                                                   iSize(0)
{
	;;
}

//
// Get number of elements
//
template <class T, typename CMP, typename DataAllocator> UINT_64 AVLTree<T, CMP, DataAllocator>::Size() { return iSize; }

//
// Destructor
//
template <class T, typename CMP, typename DataAllocator>AVLTree<T, CMP, DataAllocator>::~AVLTree() throw()
{
	// Free all data
	FreeRec(pRootNode);
}

template <class T, typename CMP, typename DataAllocator> void AVLTree<T, CMP, DataAllocator>::FreeRec(AVLTreeNode<T>  * pNode)
{
	if (pNode == NULL) { return; }

	FreeRec(pNode -> left);
	FreeRec(pNode -> right);

	--iSize;
	pNode -> ~AVLTreeNode<T>();
	pDataAllocator -> Deallocate(pNode);
}

//
// Remove key from tree
//
template <class T, typename CMP, typename DataAllocator> T AVLTree<T, CMP, DataAllocator>::Remove(const T  & oKey)
{
	CHAR_8 iChangeFlag;
	bool   bRemoved;
return RemoveRec(oKey, pRootNode, iChangeFlag, bRemoved);
}

//
// Remove key from tree
//
template <class T, typename CMP, typename DataAllocator> T AVLTree<T, CMP, DataAllocator>::Remove(const T  & oKey,
                                                                                                  bool     & bRemoved)
{
	CHAR_8 iChangeFlag;

return RemoveRec(oKey, pRootNode, iChangeFlag, bRemoved);
}

//
// Recursive remove key
//
template <class T, typename CMP, typename DataAllocator> T AVLTree<T, CMP, DataAllocator>::RemoveRec(const T           & oKey,
                                                                                                     AVLTreeNode<T>  * & pNode,
                                                                                                     CHAR_8            & iChangeFlag,
                                                                                                     bool              & bRemoved,
                                                                                                     CHAR_8              iCompareType)
{
	// Empty tree?
	if (pNode == NULL)
	{
		iChangeFlag = 0;
		bRemoved = false;
		return T();
	}

	bRemoved = true;
	T oFoundKey = T();

	char iBFResult;
	if (iCompareType == 0)
	{
		if      (!(pCMP -> operator()(pNode -> key, oKey)) &&
			 !(pCMP -> operator()(oKey, pNode -> key)))
		{
			iBFResult = 0;
		}
		else if (pCMP -> operator()(pNode -> key, oKey))
		{
			iBFResult =  1;
		}
		else
		{
			iBFResult = -1;
		}
	}
	else
	{
		iBFResult = (pNode -> left == NULL) ? 0 : -1;
	}

	char iBF = 0;
	if (iBFResult != 0)
	{
		if (iBFResult == -1) { oFoundKey = RemoveRec(oKey, pNode -> left,  iChangeFlag, bRemoved, iCompareType); }
		else                 { oFoundKey = RemoveRec(oKey, pNode -> right, iChangeFlag, bRemoved, iCompareType); }

		if (!bRemoved) { return oFoundKey; }

		iBF = iBFResult * iChangeFlag;
	}
	else
	{
		oFoundKey = pNode -> key;

		if ((pNode -> left == NULL) && (pNode -> right == NULL))
		{
			--iSize;
			pNode -> ~AVLTreeNode<T>();
			pDataAllocator -> Deallocate(pNode);

			bRemoved = true;
			pNode = NULL;

			iChangeFlag = 1;
			return oFoundKey;
		}
		else if ((pNode -> left == NULL) || (pNode -> right == NULL))
		{
			AVLTreeNode<T> * pRemoveNode = pNode;

			if (pNode -> right != NULL) { pNode = pNode -> right; }
			else                        { pNode = pNode -> left; }

			iChangeFlag = 1;

			pRemoveNode -> left = pRemoveNode -> right = NULL;

			--iSize;
			pRemoveNode -> ~AVLTreeNode<T>();
			pDataAllocator -> Deallocate(pRemoveNode);

			bRemoved = true;
			return oFoundKey;
		}
		else
		{
			pNode -> key = RemoveRec(oKey, pNode -> right, iBF, bRemoved, -1);
		}
	}

	// Change balace factor
	pNode -> bf -= iBF;

	if (iBF)
	{
		if (pNode -> bf) { iChangeFlag = Rebalance(pNode); }
		else { iChangeFlag = 1; }
	}
	else { iChangeFlag = 0; }

return oFoundKey;
}

//
// Search in tree
//
template <class T, typename CMP, typename DataAllocator> T * AVLTree<T, CMP, DataAllocator>::Find(const T  & oKey)
{
	AVLTreeNode<T>  * pNode = pRootNode;
	while (pNode != NULL)
	{
		// Key >= node value
		if (!pCMP -> operator()(oKey, pNode -> key))
		{
			// Key == node value
			if (! pCMP -> operator()(pNode -> key, oKey)) { return &(pNode -> key); }

			pNode = pNode -> right;
		}
		// Key < node value
		else
		{
			pNode = pNode -> left;
		}
	}

return NULL;
}

//
// Search least upper bound element
//
template <class T, typename CMP, typename DataAllocator>AVLTreeNode<T> * AVLTree<T, CMP, DataAllocator>::SupremumR(const T         & oKey,
                                                                                                                   AVLTreeNode<T>  * pNode)
{
	if (pNode == NULL) { return NULL; }

	AVLTreeNode<T>  * pNewNode = NULL;

	//  node >= key
	if (!pCMP -> operator()(pNode -> key, oKey))
	{
		pNewNode = SupremumR(oKey, pNode -> left);
		if (pNewNode == NULL) { return pNode; }
	}
	// node < key
	else
	{
		pNewNode = SupremumR(oKey, pNode -> right);
	}

return pNewNode;
}

//
// Search least upper bound element
//
template <class T, typename CMP, typename DataAllocator>AVLTreeNode<T> * AVLTree<T, CMP, DataAllocator>::UpperBoundR(const T         & oKey,
                                                                                                                     AVLTreeNode<T>  * pNode)
{
	if (pNode == NULL) { return NULL; }

	AVLTreeNode<T>  * pNewNode = NULL;

	//  k < node
	if (pCMP -> operator()(oKey, pNode -> key))
	{
//fprintf(stderr, "(key %d >= %d) ", oKey, pNode -> key);

		pNewNode = UpperBoundR(oKey, pNode -> left);
		if (pNewNode == NULL) { return pNode; }
	}
	// k >= node
	else
	{
//fprintf(stderr, "(key %d < %d) ", oKey, pNode -> key);
		pNewNode = UpperBoundR(oKey, pNode -> right);
	}

return pNewNode;
}

//
// Search least upper bound element
//
template <class T, typename CMP, typename DataAllocator> T * AVLTree<T, CMP, DataAllocator>::Supremum(const T  & oKey)
{
	AVLTreeNode<T> * oFoundKey = SupremumR(oKey, pRootNode);

	if (oFoundKey) { return &(oFoundKey -> key); }

return NULL;
}

//
// Search least upper bound element
//
template <class T, typename CMP, typename DataAllocator> T * AVLTree<T, CMP, DataAllocator>::UpperBound(const T  & oKey)
{
	AVLTreeNode<T> * oFoundKey = UpperBoundR(oKey, pRootNode);

	if (oFoundKey) { return &(oFoundKey -> key); }

return NULL;
}

//
// Search least upper bound element
//
template <class T, typename CMP, typename DataAllocator>AVLTreeNode<T> * AVLTree<T, CMP, DataAllocator>::LowerBoundR(const T         & oKey,
                                                                                                                     AVLTreeNode<T>  * pNode)
{
	if (pNode == NULL) { return NULL; }

	AVLTreeNode<T>  * pNewNode = NULL;

	//  k < node
	if (pCMP -> operator()(pNode -> key, oKey))
	{
//fprintf(stderr, "(key %d >= %d) ", oKey, pNode -> key);

		pNewNode = LowerBoundR(oKey, pNode -> right);
		if (pNewNode == NULL) { return pNode; }
	}
	// k >= node
	else
	{
//fprintf(stderr, "(key %d < %d) ", oKey, pNode -> key);
		pNewNode = LowerBoundR(oKey, pNode -> left);
	}

return pNewNode;
}

//
// Search least upper bound element
//
template <class T, typename CMP, typename DataAllocator>AVLTreeNode<T> * AVLTree<T, CMP, DataAllocator>::InfimumR(const T         & oKey,
                                                                                                                  AVLTreeNode<T>  * pNode)
{
	if (pNode == NULL) { return NULL; }

	AVLTreeNode<T>  * pNewNode = NULL;

	//  k >= node
	if (! pCMP -> operator()(oKey, pNode -> key))
	{
		pNewNode = InfimumR(oKey, pNode -> right);
		if (pNewNode == NULL) { return pNode; }
	}
	// k < node
	else
	{
		pNewNode = InfimumR(oKey, pNode -> left);
	}

return pNewNode;
}

//
// Search least lower bound element
//
template <class T, typename CMP, typename DataAllocator> T * AVLTree<T, CMP, DataAllocator>::Infimum(const T  & oKey)
{
	AVLTreeNode<T> * oFoundKey = InfimumR(oKey, pRootNode);

	if (oFoundKey) { return &(oFoundKey -> key); }

return NULL;
}

//
// Search least lower bound element
//
template <class T, typename CMP, typename DataAllocator> T * AVLTree<T, CMP, DataAllocator>::LowerBound(const T  & oKey)
{
	AVLTreeNode<T> * oFoundKey = LowerBoundR(oKey, pRootNode);

	if (oFoundKey) { return &(oFoundKey -> key); }

return NULL;
}

//
// Rotate node left
//
template <class T, typename CMP, typename DataAllocator>CHAR_8 AVLTree<T, CMP, DataAllocator>::RotateLeft(AVLTreeNode<T> * & pNode)
{
/*
           1            2
          / \          / \
         /   \    --> /   \
        2     3      4     1
       / \   / \          / \
      4   5 6   7        5   3
                            / \
                           6   7
*/

	AVLTreeNode<T> * pOldNode = pNode;

	const CHAR_8 iBF = (pNode -> right -> bf == 0) ? 0 : 1;

	pNode = pOldNode -> right;

	pOldNode -> right = pNode -> left;
	pNode    -> left  = pOldNode;

	pOldNode -> bf = -(--(pNode -> bf));

return iBF;
}

//
// Rotate node right
//
template <class T, typename CMP, typename DataAllocator>CHAR_8 AVLTree<T, CMP, DataAllocator>::RotateRight(AVLTreeNode<T> * & pNode)
{
/*
           1              3
          / \            / \
         /   \    -->   /   \
        2     3        1     7
       / \   / \      / \
      4   5 6   7    2   6
                    / \
                   4   5
*/
	AVLTreeNode<T> * pOldNode = pNode;

	const CHAR_8 iBF = (pNode -> left -> bf == 0) ? 0 : 1;

	pNode = pOldNode -> left;

	pOldNode -> left  = pNode -> right;
	pNode    -> right = pOldNode;

	pOldNode -> bf = -(++(pNode -> bf));

return iBF;
}

//
// Min
//
static CHAR_8 MIN(CHAR_8 a, CHAR_8 b) { return (a < b) ? a : b; }

//
// max
//
static CHAR_8 MAX(CHAR_8 a, CHAR_8 b) { return (a > b) ? a : b; }

//
// Duble right rotation
//
template <class T, typename CMP, typename DataAllocator> CHAR_8 AVLTree<T, CMP, DataAllocator>::DblRotateRight(AVLTreeNode<T> * & pNode)
{
/*
                    1                                       5
                   / \                                     / \
                  /   \                                   /   \
                 /     \                                 /     \
                /       \                               /       \
               /         \                             /         \
              2           3                           2           1
             / \         / \                         / \         / \
            /   \       /   \         --->          /   \       /   \
           /     \     /     \                     /     \     /     \
          4       5   6       7                   4      10   11      3
         / \     / \                             / \                 / \
        /   \   /   \                           /   \               /   \
       8     9 10   11                         8     9             6     7

*/
	AVLTreeNode<T> * pOldNode = pNode;
	AVLTreeNode<T> * pOldNodeSubTree = pNode -> left;

	pNode = pOldNode -> left -> right;

	pOldNode -> left  = pNode -> right;
	pNode    -> right = pOldNode;

	pOldNodeSubTree -> right = pNode -> left;
	pNode           -> left  = pOldNodeSubTree;

	pNode -> left -> bf  = -MAX(pNode -> bf, 0);
	pNode -> right -> bf = -MIN(pNode -> bf, 0);
	pNode -> bf = 0;

return  1;
}

//
// Duble left rotation
//
template <class T, typename CMP, typename DataAllocator> CHAR_8 AVLTree<T, CMP, DataAllocator>::DblRotateLeft(AVLTreeNode<T> * & pNode)
{
/*
                    1                                       6
                   / \                                     / \
                  /   \                                   /   \
                 /     \                                 /     \
                /       \                               /       \
               /         \                             /         \
              2           3                           1           3
             / \         / \                         / \         / \
            /   \       /   \         --->          /   \       /   \
           /     \     /     \                     /     \     /     \
          4       5   6       7                   2      12   13      7
                     / \     / \                 / \   ..........    / \
                    /   \   /   \               /   \               /   \
                   12   13 14   15             4     5             14    15

*/
	AVLTreeNode<T> * pOldNode = pNode;
	AVLTreeNode<T> * pOldNodeSubTree = pNode -> right;

	pNode = pOldNode -> right -> left;

	pOldNode -> right = pNode -> left;
	pNode    -> left  = pOldNode;

	pOldNodeSubTree -> left  = pNode -> right;
	pNode           -> right = pOldNodeSubTree;

	pNode -> left -> bf  = -MAX(pNode -> bf, 0);
	pNode -> right -> bf = -MIN(pNode -> bf, 0);
	pNode -> bf = 0;

return  1;
}

//
// Re-balance tree
//
template <class T, typename CMP, typename DataAllocator> CHAR_8 AVLTree<T, CMP, DataAllocator>::Rebalance(AVLTreeNode<T> * & pNode)
{
	CHAR_8 iHeightChange = 0;
	// Need a right rotation
	if (pNode -> bf < C_AVL_LEFT_HEAVY)
	{
		// RL rotation
		if (pNode -> left -> bf == C_AVL_RIGHT_HEAVY) { iHeightChange = DblRotateRight(pNode); }
		// RR rotation
		else                                          { iHeightChange =    RotateRight(pNode); }
	}
	// Need left rotation
	else if (pNode -> bf > C_AVL_RIGHT_HEAVY)
	{
		// LR rotation
		if (pNode -> right -> bf == C_AVL_LEFT_HEAVY) { iHeightChange = DblRotateLeft(pNode); }
		// LL rotation
		else                                          { iHeightChange =    RotateLeft(pNode); }
	}

return iHeightChange;
}

//
// Insert element into tree
//
template <class T, typename CMP, typename DataAllocator>AVLTreeNode<T> * AVLTree<T, CMP, DataAllocator>::InsertRec(const T          & oKey,
                                                                                                                   AVLTreeNode<T> * & pTree,
                                                                                                                   CHAR_8           & iNewBF)
{
	// Insert into empty tree
	if (pTree == NULL)
	{
		++iSize;
		void * vTree = pDataAllocator -> Allocate(NodeSize());
		pTree = new(vTree)AVLTreeNode<T>();

		pTree -> key   = oKey;
		pTree -> left  = NULL;
		pTree -> right = NULL;
		pTree -> bf    = 0;

		iNewBF = 1;

		return NULL;
	}

	// Element
	AVLTreeNode<T> * oFoundKey = NULL;
	CHAR_8 iBF = 0;

	// Search in left subtree
	if (pCMP -> operator()(oKey, pTree -> key))
	{
		oFoundKey = InsertRec(oKey, pTree -> left, iNewBF);
		if (oFoundKey != NULL) { return oFoundKey; }

		// Balance factor
		iBF = -iNewBF;
	}
	// Search in right subtree
	else
	{
		// Equal
		if (!pCMP -> operator()(pTree -> key, oKey)) { return pTree; }

		oFoundKey = InsertRec(oKey, pTree -> right, iNewBF);
		if (oFoundKey != NULL) { return oFoundKey; }

		// Balance factor
		iBF = iNewBF;
	}

	// Balance factor
	pTree -> bf += iBF;

	// Rebalance, if need
	iNewBF = (iBF != 0 && pTree -> bf != 0) ? (1 - Rebalance(pTree)) : 0;

return NULL;
}

//
// Insert element into tree
//
template <class T, typename CMP, typename DataAllocator> T * AVLTree<T, CMP, DataAllocator>::Insert(const T  & oKey)
{
	CHAR_8 iNewBF;
	AVLTreeNode<T> * pNode = InsertRec(oKey, pRootNode, iNewBF);

return (pNode == NULL) ? NULL : &(pNode -> key);
}

//
// Print tree
//
template <class T, typename CMP, typename DataAllocator> void AVLTree<T, CMP, DataAllocator>::Print(STLW::stringstream & oStream)
{
	Print(oStream, pRootNode, 0);
}

//
// Print tree
//
template <class T, typename CMP, typename DataAllocator> void AVLTree<T, CMP, DataAllocator>::Print(STLW::stringstream  & oStream,
                                                                                                    AVLTreeNode<T>      * pNode,
                                                                                                    const UINT_32         iLevel)
{
	if (pNode == NULL) { return; }

	Print(oStream, pNode -> left, iLevel + 1);

	for (UINT_32 iPos = 0; iPos < iLevel; ++iPos) { oStream << "    "; }

	oStream << pNode -> key;
	switch(pNode -> bf)
	{
		case -1:
			oStream << "\\\n";
			break;
		case 0:
			oStream << ":\n";
			break;
		case 1:
			oStream << "/\n";
			break;
	}

	Print(oStream, pNode -> right, iLevel + 1);
}

} // namespace IRIS
#endif // _AVL_TREE_HPP__
// End.
