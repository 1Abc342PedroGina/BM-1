/* SPDX-License-Identifier: GPL-2.0-only */
/* Modifications: Add typedefs */
#ifndef _LINUX_RBTREE_H
#define _LINUX_RBTREE_H

struct list_head {
	struct list_head *next, *prev;
};

struct rb_node {
	unsigned long  __rb_parent_color;
	struct rb_node *rb_right;
	struct rb_node *rb_left;
} __attribute__((aligned(sizeof(long))));
/* The alignment might seem pointless, but allegedly CRIS needs it */

typedef struct rb_node   rb_node_t;

struct rb_node_linked {
	struct rb_node		node;
	struct rb_node_linked	*prev;
	struct rb_node_linked	*next;
};

struct rb_root {
	struct rb_node *rb_node;
};

typedef struct rb_node_linked    rb_node_linked;
/*
 * Leftmost-cached rbtrees.
 *
 * We do not cache the rightmost node based on footprint
 * size vs number of potential users that could benefit
 * from O(1) rb_last(). Just not worth it, users that want
 * this feature can always implement the logic explicitly.
 * Furthermore, users that want to cache both pointers may
 * find it a bit asymmetric, but that's ok.
 */
struct rb_root_cached {
	struct rb_root rb_root;
	struct rb_node *rb_leftmost;
};

/*
 * Leftmost tree with links. This would allow a trivial rb_rightmost update,
 * but that has been omitted due to the lack of users.
 */
struct rb_root_linked {
	struct rb_root		rb_root;
	struct rb_node_linked	*rb_leftmost;
};

#define RB_ROOT (struct rb_root) { NULL, }
#define RB_ROOT_CACHED (struct rb_root_cached) { {NULL, }, NULL }
#define RB_ROOT_LINKED (struct rb_root_linked) { {NULL, }, NULL }

struct ns_tree_root {
	struct rb_root ns_rb;
	struct list_head ns_list_head;
};

typedef struct {
	int __aligned(sizeof(int)) counter;
} atomic_t;

struct ns_tree {
	uint64_t ns_id;
	atomic_t __ns_ref_active;
	struct ns_tree_node ns_unified_node;
	struct ns_tree_node ns_tree_node;
	struct ns_tree_node ns_owner_node;
	struct ns_tree_root ns_owner_root;
};

#endif
