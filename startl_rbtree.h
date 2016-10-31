//
// Created by simanstar on 16-10-20.
//

#ifndef STARTL_STARTL_RBTREE_H
#define STARTL_STARTL_RBTREE_H

#include "memory.h"
#include "startl_pair.h"
#include "startl_algobase.h"

namespace startl {

typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;
const __rb_tree_color_type __rb_tree_black = true;
///////////////////////////////////////////////////////////////////node

struct __rb_tree_node_base {
  typedef __rb_tree_color_type color_type;
  typedef __rb_tree_node_base *base_ptr;

  color_type color;
  base_ptr parent;
  base_ptr left;
  base_ptr right;

  static base_ptr minimum(base_ptr x) {
    while (x->left != 0) x = x->left;
    return x;
  }

  static base_ptr maximum(base_ptr x) {
    while (x->right != 0) x = x->right;
    return x;
  }
};

template<class Value>
struct __rb_tree_node : public __rb_tree_node_base {
  typedef __rb_tree_node<Value> *link_type;
  Value value_field;
};

/////////////////////////////////////////////////////////////////////////迭代器

struct __rb_tree_iterator_base {
  typedef __rb_tree_node_base::base_ptr base_ptr;
  typedef bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;
  base_ptr node;                                        //node为什么这个类型，直接在__rb_tree_iterator里link_type类型不就好了？？？

  void increment() {                                  //++end()为最后一个元素？？？
    if (node->right != nullptr) {                     //有右子节点
      node = node->right;                             //就向右走
      while (node->left != nullptr)                   //然后一直往左子树走到底
        node = node->left;                            //即是解答
    }
    else {                                            //没有右子节点
      base_ptr y = node->parent;                      //找出父节点
      while (node == y->right) {                      //如果现行节点本身是个右子节点
        node = y;                                     //就一直上溯，直到“不为右子节点”为止
        y = y->parent;
      }
      if (node->right != y)                           //若此时的右子节点不等于此时的父节点
        node = y;                                     //--begin() 为 end()  ;   ++最后一个元素为　end()  ;
    }

  }

  void decrement() {
    if (node->color == __rb_tree_red && node->parent->parent == node)           //该情况发生于node为header时，即node为end()
      node = node->right;
    else if (node->left != nullptr) {                                           //如果有左子节点
      base_ptr y = node->left;
      while (y->right != nullptr)
        y = y->right;
      node = y;
    }
    else {                                                                      //找父结点
      base_ptr y = node->parent;                                                //当现行节点身为左子节点
      while (node == y->left) {                                                 // 一直交替往上走，直到现行节点
        node = y;                                                               //不为左子节点
        y = y->parent;
      }
      node = y;                                                                 //此时父结点即为答案，为什么和加不一样？？？
    }
  }
};

template <class T>
struct __rb_tree_iterator : public __rb_tree_iterator_base {
  typedef T value_type;
  typedef value_type* pointer;
  typedef value_type& reference;
  typedef __rb_tree_iterator<T> iterator;
  typedef __rb_tree_iterator<const T> const_iterator;
  typedef __rb_tree_iterator self;                       //原为__rb_tree_iterator<T> self，改为__rb_tree_iterator是否可行？ 可行!
  typedef __rb_tree_node<T>* link_type;

  __rb_tree_iterator() {}
  __rb_tree_iterator(link_type x) { node = x;}
  template <class U>
  __rb_tree_iterator(const __rb_tree_iterator<U>& it) {node = it.node;}

  reference operator*() const {return link_type(node)->value_field; }
  pointer operator->() const { return &(operator*()); }
  self& operator++() {increment(); return *this;}
  self operator++(int) {
    self tmp = *this;
    increment();
    return tmp;
  }
  self& operator--() {decrement(); return *this;}
  self operator--(int) {
    self tmp = *this;
    decrement();
    return tmp;
  }

  bool operator==(const self& atr) {
    return node == atr.node;
  }

  bool operator!=(const self& atr) {
    return node != atr.node;
  }




};

/////////////////////////////////////////////////////////////////rb_tree 定义
template <class Key, class Value, class KeyOfValue, class Compare, template<class E> class Alloc = Allocator>
class rb_tree {
protected:
  typedef void* void_pointer;
  typedef __rb_tree_node_base* base_ptr;
  typedef __rb_tree_node<Value> rb_tree_node;
  typedef Alloc<rb_tree_node> rb_tree_node_allocator;
  typedef __rb_tree_color_type color_type;

public:
  typedef Key key_type;
  typedef Value value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type* reverse_iterator;
  typedef const value_type* const_reverse_iterator;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef rb_tree_node* link_type;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef __rb_tree_iterator<value_type> iterator;
  typedef __rb_tree_iterator<const value_type> const_iterator;
  typedef rb_tree self;                         //这样可以否？？？

  link_type get_node() {return rb_tree_node_allocator::allocate(); }
  void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }

  link_type create_node(const value_type& x) {
    link_type tmp = get_node();
    try {
      construct(&tmp->value_field, x);
    }
    catch (...) {
      put_node(tmp);                         //没有调用析构！因为是构造失败后的
      throw;
    }
    return tmp;
  }

  link_type clone_node(link_type x) {
    link_type tmp = create_node(x->value_field);
    tmp->color = x->color;
    tmp->left = nullptr;
    tmp->right = nullptr;
    return tmp;
  }

  void destroy_node(link_type p) {
    destroy(&p->value_field);
    put_node(p);
  }

protected:
  size_type node_count;
  link_type header;
  Compare key_compare;
  //以下三个函数用来取header成员
  link_type& root() const {return (link_type&) header->parent;}
  link_type& leftmost() const {return (link_type&) header->left;}
  link_type& rightmost() const {return (link_type&) header->right;}
  //取x的成员
  static link_type& left(link_type x) {return (link_type&)(x->left);}
  static link_type & right(link_type x) {return (link_type&)(x->right);}
  static link_type & parent(link_type x) { return (link_type&)(x->parent);}
  static reference value(link_type x) {return x->value_field;}
  static const Key& key(link_type x) {return KeyOfValue()(value(x));}
  static color_type & color(link_type x) {return (color_type&)(x->color);}
  //取x的成员
  static link_type& left(base_ptr x) {return (link_type&)(x->left);}
  static link_type & right(base_ptr x) {return (link_type&)(x->right);}
  static link_type & parent(base_ptr x) { return (link_type&)(x->parent);}
  static reference value(base_ptr x) {return ((link_type)x)->value_field;}
  static const Key& key(base_ptr x) {return KeyOfValue()(value(link_type(x)));}
  static color_type & color(base_ptr x) {return (color_type&)((link_type (x))->color);}

  static link_type minimum(link_type x) {
    return (link_type) __rb_tree_node_base::minimum(x);
  }

  static link_type maximum(link_type x) {
    return (link_type) __rb_tree_node_base::maximum(x);
  }

private:
  iterator __insert(base_ptr x, base_ptr y, const value_type& v);
  link_type __copy(link_type x, link_type p);
  void __erase(link_type x);
  void init() {
    header = get_node();
    color(header) = __rb_tree_red;
    root() = 0;
    leftmost() = header;
    rightmost() = header;
  }

  inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
    x->color = __rb_tree_red;       //新节点必为红
    while (x != root && x->parent->color == __rb_tree_red) {
      if (x->parent == x->parent->parent->left) {           //父节点为祖父节点之左子节点
        __rb_tree_node_base* y = x->parent->parent->right;
        if (y && y->color == __rb_tree_red) {               //伯父节点存在，且为红
          x->parent->color = __rb_tree_black;
          y->color = __rb_tree_black;
          x->parent->parent->color = __rb_tree_red;
          x = x->parent->parent;
        } else {                                             //无伯父节点,或伯父节点为黑
           if (x == x->parent->right) {
            x = x->parent;
            __rb_tree_rotate_left(x, root);
          }
          x->parent->color = __rb_tree_black;
          x->parent->parent->color = __rb_tree_red;
          __rb_tree_rotate_right(x->parent->parent, root);
        }
      }
      else {                                                //父节点为祖父节点之右子节点
        __rb_tree_node_base* y = x->parent->parent->left;
        if (y && y->color == __rb_tree_red) {               //有伯父节点，且为红
          x->parent->color = __rb_tree_black;
          y->color = __rb_tree_black;
          x->parent->parent->color = __rb_tree_red;
          x = x->parent->parent;                            //准备继续往上层检查
        } else {
            if (x == x->parent->left) {
              x = x->parent;
              __rb_tree_rotate_right(x, root);
            }
          x->parent->color = __rb_tree_black;
          x->parent->parent->color = __rb_tree_red;
          __rb_tree_rotate_left(x->parent->parent, root);
        }
      }
    }
    root->color = __rb_tree_black;
  }

  inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {    //root会改变，所以是指针引用
    //x为旋转点
    __rb_tree_node_base* y = x->right;
    x->right = y->left;
    if (y->left != nullptr)
      y->left->parent = x;
    y->parent = x->parent;
    if (x == root)
      root = y;
    else if (x == x->parent->left)
      x->parent->left = y;
    else
      x->parent->right = y;
    y->left = x;
    x->parent = y;
  }

  inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
    //x为旋转点
    __rb_tree_node_base* y = x->left;
    x->left = y->right;
    if (y->right != nullptr)
      y->right->parent = x;
    y->parent = x->parent;
    if (x == root)
      root = y;
    else if (x == x->parent->right)
      x->parent->right = y;
    else
      x->parent->left = y;
    y->right = x;
    x->parent = y;
  }

  __rb_tree_node_base* __rb_tree_rebalance_for_erase(__rb_tree_node_base* z, __rb_tree_node_base*& root,
                                                     __rb_tree_node_base*& leftmost, __rb_tree_node_base*& rightmost);

  void distance(iterator first, iterator last, size_type & n) const {
    for (n = 0; first != last; ++first)
      ++n;
  }

  bool _compare_tree(const link_type & st1, const link_type & st2);
public:
  rb_tree(const Compare& comp = Compare())
      : node_count(0), key_compare(comp) {init();}
  ~rb_tree() {
    clear();
    put_node(header);
  }

  bool operator==(const self& x) {
    if (node_count != x.node_count)
      return 0;
    return _compare_tree(root(), x.root());
  }

  Compare key_comp() const {return key_compare;}
  iterator begin() const {return leftmost();}
  iterator end() const {return header;}
  reverse_iterator rbegin() {return rightmost();}
  reverse_iterator rend() {return header;}
  bool empty() const {return node_count == 0;}
  size_type size() const {return node_count;}
  size_type max_size() const {return size_type (-1);}

  pair<iterator, bool> insert_unique(const value_type& x);
  iterator insert_equal(const value_type& x);
  iterator find(const Key& k) const ;

  void swap(self& atr) {
    __startl::swap(header, atr.header);
    __startl::swap(node_count, atr.node_count);
    __startl::swap(key_compare, atr.key_compare);
  }
  //删除指定位置节点
  void erase(iterator position) {
    link_type y = (link_type)__rb_tree_rebalance_for_erase(position.node, header->parent, header->left, header->right);
    destroy_node(y);
    --node_count;
  }
  //删除key值为k所有节点
  size_type erase(const key_type& k) {
    size_type n = 0;
    pair<iterator, iterator> p = equal_range(k);
    distance(p.first, p.second, n);
    if (n  > 0)
      erase(p.first, p.second);
    return n;

  }

  void erase(iterator first, iterator last) {
    if (first == begin() && last == end())
      clear();
    else
      while (first != last) erase(first++);
  }

  void clear() {
    if (node_count != 0) {
      __erase(root());
      leftmost() = header;
      root() = 0;
      rightmost() = header;
      node_count = 0;
    }
  }

  iterator lower_bound(const key_type& k) const {
    link_type y = header;
    link_type x = root();

    while ( x != nullptr)
      if (!key_compare(key(x), k))
        y = x, x = left(x);
      else
        x = right(x);
    return iterator(y);
  }

  iterator upper_bound(const key_type& k) const {
    link_type y = header;
    link_type x = root();

    while ( x != nullptr)
      if (key_compare(k, key(x)))
        y = x, x = left(x);
      else
        x = right(x);
    return iterator(y);
  }

  pair<iterator, iterator> equal_range(const key_type& x) const {
    return pair<iterator, iterator>(lower_bound(x), upper_bound(x));
  }

  template <class InputIterator>
  void insert_unique(InputIterator first, InputIterator last);
  size_type count(const key_type &k) const ;

};

///////////////////////////////////////////////////////////////////////////////////////////////////实现
template <class Key, class Value, class KeyOfValue, class Compare, template<class E> class Alloc >
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type &v) {
  link_type y = header;
  link_type x = root();
  while (x != nullptr) {
    y = x;
    x = key_compare(key(x), KeyOfValue()(v)) ? right(x) : left(x);
  }
  return __insert(x, y, v);
}

template <class Key, class Value, class KeyOfValue, class Compare, template<class E> class Alloc >
pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,bool>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type &v) {
  link_type y = header;
  link_type x = root();
  bool comp = true;
  while (x != nullptr) {
    y = x;
    comp = key_compare(KeyOfValue()(v), key(x));
    x = comp ? left(x) : right(x);
  }
  iterator j = iterator(y);
  if (comp) {
    if (j == begin())
      return pair<iterator, bool>(__insert(x, y, v), true);
    else
      --j;                                      //不是最左不能插入？？？
  }
  if (key_compare(key(j.node), KeyOfValue()(v)))
    return pair<iterator, bool>(__insert(x, y, v), true);
  return pair<iterator, bool>(j, false);
}

template <class Key, class Value, class KeyOfValue, class Compare, template<class E> class Alloc >
template <class InputIterator>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(InputIterator first, InputIterator last) {
  for (; first != last; ++first)
    insert_unique(*first);
}

template <class Key, class Value, class KeyOfValue, class Compare, template<class E> class Alloc >
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr x_, base_ptr y_, const value_type &v) {
  link_type x = (link_type) x_;
  link_type y = (link_type) y_;
  link_type z;
  if (y == header || x != nullptr /*x还有不等于nullptr的情况？？？*/ || key_compare(KeyOfValue()(v), key(y))) { //新节点在左边
    z = create_node(v);
    left(y) = z;
    if (y == header) {
      root() = z;
      rightmost() = z;
    } else if (y == leftmost()) {
      leftmost() = z;
    }
  }
  else {                                                  //新节点在右边
    z = create_node(v);
    right(y) = z;
    if (y == rightmost())
      rightmost() = z;
  }
  parent(z) = y;
  left(z) = nullptr;
  right(z) = nullptr;

  __rb_tree_rebalance(z, header->parent);
  ++node_count;
  return iterator(z);
}

template <class Key, class Value, class KeyOfValue, class Compare, template<class E> class Alloc >
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key &k) const {
  link_type y = header;
  link_type x = root();

  while ( x != nullptr)                                                     //确定不用花括号？
    if (!key_compare(key(x), k))
      y = x, x = left(x);
    else
      x = right(x);
  iterator j = iterator(y);
  return (j == end() || key_compare(k, key(j.node))) ? end() : j;           //如果不加j == end()条件,j为end时j.node就会报错
}

template <class Key, class Value, class KeyOfValue, class Compare, template<class E> class Alloc >
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::count(const key_type &k) const {
  pair<const_iterator, const_iterator> p = equal_range(k);
  size_type n = 0;
  distance(p.first, p.second, n);
  return n;
}


template <class Key, class Value, class KeyOfValue, class Compare, template<class E> class Alloc >
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__erase(link_type x) {
  while (x != nullptr) {
    __erase(right(x));
    link_type y = left(x);
    destroy_node(x);
    x = y;
  }
}
//删除结点z
template <class Key, class Value, class KeyOfValue, class Compare, template<class E> class Alloc >
 __rb_tree_node_base*
 rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__rb_tree_rebalance_for_erase(__rb_tree_node_base* z,
                              __rb_tree_node_base*& root,
                              __rb_tree_node_base*& leftmost,
                              __rb_tree_node_base*& rightmost)
{
  __rb_tree_node_base* y = z;
  __rb_tree_node_base* x = 0;
  __rb_tree_node_base* x_parent = 0;
  if (y->left == 0)             // z has at most one non-null child. y == z.
    x = y->right;               // x might be null.
  else
  if (y->right == 0)          // z has exactly one non-null child.  y == z.
    x = y->left;              // x is not null.
  else {                      // z has two non-null children.  Set y to
    y = y->right;             //   z's successor.  x might be null.
    while (y->left != 0)
      y = y->left;
    x = y->right;
  }
  if (y != z) {                 // relink y in place of z.  y is z's successor
    z->left->parent = y;
    y->left = z->left;
    if (y != z->right) {
      x_parent = y->parent;
      if (x) x->parent = y->parent;
      y->parent->left = x;      // y must be a left child
      y->right = z->right;
      z->right->parent = y;
    }
    else
      x_parent = y;
    if (root == z)
      root = y;
    else if (z->parent->left == z)
      z->parent->left = y;
    else
      z->parent->right = y;
    y->parent = z->parent;
    __startl::swap(y->color, z->color);
    y = z;
    // y now points to node to be actually deleted
  }
  else {                        // y == z
    x_parent = y->parent;
    if (x) x->parent = y->parent;
    if (root == z)
      root = x;
    else
    if (z->parent->left == z)
      z->parent->left = x;
    else
      z->parent->right = x;
    if (leftmost == z)
    if (z->right == 0)        // z->left must be null also
      leftmost = z->parent;
      // makes leftmost == header if z == root
    else
      leftmost = __rb_tree_node_base::minimum(x);
    if (rightmost == z)
    if (z->left == 0)         // z->right must be null also
      rightmost = z->parent;
      // makes rightmost == header if z == root
    else                      // x == z->left
      rightmost = __rb_tree_node_base::maximum(x);
  }
  if (y->color != __rb_tree_red) {
    while (x != root && (x == 0 || x->color == __rb_tree_black))
      if (x == x_parent->left) {
        __rb_tree_node_base* w = x_parent->right;
        if (w->color == __rb_tree_red) {
          w->color = __rb_tree_black;
          x_parent->color = __rb_tree_red;
          __rb_tree_rotate_left(x_parent, root);
          w = x_parent->right;
        }
        if ((w->left == 0 || w->left->color == __rb_tree_black) &&
            (w->right == 0 || w->right->color == __rb_tree_black)) {
          w->color = __rb_tree_red;
          x = x_parent;
          x_parent = x_parent->parent;
        } else {
          if (w->right == 0 || w->right->color == __rb_tree_black) {
            if (w->left) w->left->color = __rb_tree_black;
            w->color = __rb_tree_red;
            __rb_tree_rotate_right(w, root);
            w = x_parent->right;
          }
          w->color = x_parent->color;
          x_parent->color = __rb_tree_black;
          if (w->right) w->right->color = __rb_tree_black;
          __rb_tree_rotate_left(x_parent, root);
          break;
        }
      } else {                  // same as above, with right <-> left.
        __rb_tree_node_base* w = x_parent->left;
        if (w->color == __rb_tree_red) {
          w->color = __rb_tree_black;
          x_parent->color = __rb_tree_red;
          __rb_tree_rotate_right(x_parent, root);
          w = x_parent->left;
        }
        if ((w->right == 0 || w->right->color == __rb_tree_black) &&
            (w->left == 0 || w->left->color == __rb_tree_black)) {
          w->color = __rb_tree_red;
          x = x_parent;
          x_parent = x_parent->parent;
        } else {
          if (w->left == 0 || w->left->color == __rb_tree_black) {
            if (w->right) w->right->color = __rb_tree_black;
            w->color = __rb_tree_red;
            __rb_tree_rotate_left(w, root);
            w = x_parent->left;
          }
          w->color = x_parent->color;
          x_parent->color = __rb_tree_black;
          if (w->left) w->left->color = __rb_tree_black;
          __rb_tree_rotate_right(x_parent, root);
          break;
        }
      }
    if (x) x->color = __rb_tree_black;
  }
  return y;
}

template <class Key, class Value, class KeyOfValue, class Compare, template<class E> class Alloc >
bool rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_compare_tree(const link_type & st1, const link_type & st2) {
  if (st1 == nullptr || st2 == nullptr) {
    return st1 == nullptr && st2 == nullptr ? 1 : 0 ;
  }
  if (st1->value_field == st2->value_field) {
    return _compare_tree((link_type)st1->left, (link_type)st2->left) &&
           _compare_tree((link_type)st1->right, (link_type)st2->right);
  }
}

}
#endif //STARTL_STARTL_RBTREE_H
