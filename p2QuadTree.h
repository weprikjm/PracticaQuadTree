// ----------------------------------------------------
// Quadtree implementation --
// ----------------------------------------------------

#ifndef __P2QUADTREE_H__
#define __P2QUADTREE_H__

#include "Collider.h"
#include "p2DynArray.h"
#include "Globals.h"
#define QUADTREE_MAX_ITEMS 1

// Helper function to check if one rectangle complately contains another
bool Contains(const SDL_Rect& a, const SDL_Rect& b);
bool Intersects(const SDL_Rect& a, const SDL_Rect& b);

// Tree node -------------------------------------------------------
class p2QuadTreeNode
{

public:

	SDL_Rect				rect;
	p2DynArray<Collider*>	objects;
	p2QuadTreeNode*			parent;
	p2QuadTreeNode*			childs[4];

public:

	p2QuadTreeNode(SDL_Rect r) : 
		rect(r),
		objects(QUADTREE_MAX_ITEMS)
	{
		parent = childs[0] = childs[1] = childs[2] = childs[3] = NULL;
	}

	p2QuadTreeNode(){}

	~p2QuadTreeNode()
	{
		for(int i = 0; i < 4; ++i)
			if(childs[i] != NULL) delete childs[i];
	}
	 
	void Insert(Collider* col)
	{
	
	
		// TODO: Insertar un nou Collider al quadtree
		if (objects.Count()<QUADTREE_MAX_ITEMS && childs[0] == NULL)
		{
				objects.PushBack(col);
		}
		else
		{

			if (Intersects(childs[0]->rect, col->rect) && Intersects(childs[1]->rect, col->rect) && Intersects(childs[2]->rect, col->rect) && Intersects(childs[3]->rect, col->rect))
				parent->objects.PushBack(col);
			else
			{

				if (childs[0] == NULL)
				{
					SDL_Rect rect1;
					rect1.x = rect.x;
					rect1.y = rect.y;
					rect1.h = rect.h / 2;
					rect1.w = rect.w / 2;

					SDL_Rect rect2;
					rect2.x = rect.x + rect.w / 2;
					rect2.y = rect.y;
					rect2.h = rect.h / 2;
					rect2.w = rect.w / 2;


					SDL_Rect rect3;
					rect3.x = rect.x;
					rect3.y = rect.y + rect.h / 2;
					rect3.h = rect.h / 2;
					rect3.w = rect.w / 2;


					SDL_Rect rect4;
					rect4.x = rect.x + rect.w / 2;
					rect4.y = rect.y + rect.h / 2;
					rect4.h = rect.h / 2;
					rect4.w = rect.w / 2;

					childs[0] = new p2QuadTreeNode(rect1);
					childs[1] = new p2QuadTreeNode(rect2);
					childs[2] = new p2QuadTreeNode(rect3);
					childs[3] = new p2QuadTreeNode(rect4);

				}
				for (int i = 0; i < 4; i++)
				{
					childs[i]->parent = this;

					if (Intersects(childs[i]->rect, col->rect))
					{
						childs[i]->Insert(col);
					}

				}

			}

		}
		






	}

	int CollectCandidates(p2DynArray<Collider*>& nodes, const SDL_Rect& r) 
	{
		int possibleCollisions = 0;
		
		// TODO:
		// Omplir el array "nodes" amb tots els colliders candidats
		// de fer intersecció amb el rectangle r
		// retornar el número de intersección calculades en el procés
		// Nota: és una funció recursiva
		/*

		p2QuadTreeNode* tmp;
		tmp = this;

		for (int i = 0; i < 4; i++)
		{
			if (tmp->childs[0] != NULL && Contains(childs[i]->rect, r) && tmp != NULL)
			{
				tmp = childs[i];
				tmp->CollectCandidates(nodes, r);
			}
			
			
		}
		int x = 0;

		while (x < objects.Count())
		{
			nodes.PushBack(tmp->objects[x]);
			x++;
		}

		possibleCollisions = x;
		*/
		return possibleCollisions;
		
	}

	void CollectRects(p2DynArray<p2QuadTreeNode*>& nodes) 
	{
		nodes.PushBack(this);

		for(int i = 0; i < 4; ++i)
			if(childs[i] != NULL) childs[i]->CollectRects(nodes);
	}

};

// Tree class -------------------------------------------------------
class p2QuadTree
{
public:

	// Constructor
	p2QuadTree() : root(NULL)
	{

	}

	// Destructor
	virtual ~p2QuadTree()
	{
		Clear();
	}

	void SetBoundaries(const SDL_Rect& r)
	{
		if(root != NULL)
			delete root;

		root = new p2QuadTreeNode(r);
	}

	void Insert(Collider* col)
	{
		if(root != NULL)
		{
			if(Intersects(root->rect, col->rect))
				root->Insert(col);
		}

	}

	void Clear()
	{
		if(root != NULL)
		{
			delete root;
			root = NULL;
		}
	}

	int CollectCandidates(p2DynArray<Collider*>& nodes, const SDL_Rect& r) const
	{
		int tests = 0;
		if(root != NULL && Intersects(root->rect, r))
			tests = root->CollectCandidates(nodes, r);
		return tests;
	}

	void CollectRects(p2DynArray<p2QuadTreeNode*>& nodes)const
	{
		if(root != NULL)
			root->CollectRects(nodes);
	}

public:

	p2QuadTreeNode*	root;

};

#endif // __p2QuadTree_H__