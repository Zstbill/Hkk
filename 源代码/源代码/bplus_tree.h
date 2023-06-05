#ifndef BIN_BPLUS_TREE_H
#define BIN_BPLUS_TREE_H


#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>

using namespace std;

// 初始化为五十阶BPlus_Tree
// 可以很方便的修改
const int ORDER = 50;
const int NODE_MINIMUM_DEGREE = ceil(ORDER / 2);

template<typename key_type, typename value_type>
class BPlus_tree_node
{
public:
    // leaf_node的属性：①②③⑤⑥⑦
    // index_node的属性：①②④⑤
    bool is_leaf;                               // ①是否为叶子节点
    vector<key_type> keys;                      // ②关键字
    vector<value_type> values;                      // ③值
    vector<BPlus_tree_node *> children;          // ④孩子节点
    BPlus_tree_node *parent = nullptr;          // ⑤父亲节点
    BPlus_tree_node *next = nullptr;            // ⑥右兄弟节点
    BPlus_tree_node *left = nullptr;            // ⑦左兄弟节点

    // 初始化一个新节点时必须定义其是否为叶子节点
    explicit BPlus_tree_node(bool a) : is_leaf(a)
    {};

    // 清空节点，防止内存泄露
    void clear();
};

// 节点清空操作
template<typename key_type, typename value_type>
void BPlus_tree_node<key_type, value_type>::clear()
{
    next = nullptr;
    left = nullptr;
    parent = nullptr;
    children.clear();
    values.clear();
    keys.clear();
    is_leaf = false;
}

template<typename key_type, typename value_type>
class BPlus_tree
{
private:
    BPlus_tree_node<key_type, value_type> *root = nullptr;    // 初始化时为空树
public:
    BPlus_tree() = default;

    // 对外接口：插入函数
    // 如果为空树则直接新建根节点
    // 如果不为空树则调用内部查找函数
    void insert_value(key_type key, const value_type &value);

    // 对外接口：查找函数
    // 如果为叶子节点则直接在当前节点找到key
    // 如果不是叶子节点则执行内部查找函数
    // 执行内部查找函数后若搜索到叶子节点则返回value值
    value_type search_value(key_type key);

    // 对外接口：删除函数
    // 传入key值，找到对应的value将其删除
    // 需要先调用找叶子节点的函数 find_key_node
    // 返回值为true则删除成功
    bool drop_value(key_type key);

    // 对外接口：修改函数
    // 如果为当前节点为叶子节点则直接在当前节点执行修改值的操作
    // 返回true则修改成功
    // 返回false则修改失败（没有找到该key）
    bool update(key_type key, value_type value);

    //对外接口：输出函数
    void print();

    // 对外接口：查找key是否存在
    bool if_key_exist(key_type key);

    // 对外接口：查找key区间内的value(使用于辅助索引,key是数字,value是主键)
    vector<key_type> search_range(key_type min_key, key_type max_key);

    // 对外接口：查找key区间内的value(使用于辅助索引,key是数字,value是主键)
    vector<key_type> search_range(key_type key, bool up);


private:
    // 中间函数
    // 在keys列表中找key对应的索引
    // 如果没找到则返回-1
    // 如果找到则返回该key对应的索引值
    // 该函数设置为全局函数
    // int find_key_index(const vector<key_type>& keys, key_type key);

    // 中间函数
    // 找key对应的索引位置的函数
    // 找到该关键字应插入的位置
    // 简单的二分查找
    // 返回值即为key对应的索引位置
    // 该函数设置为全局函数
    // int find_key_pos(const vector<key_type>&, key_type key);

    // 查找key所在的叶子节点的函数
    // 返回值为所寻找的叶子节点
    // 便于函数实现的中间函数
    BPlus_tree_node<key_type, value_type> *find_key_node(key_type key);

    // 内部插入函数
    // 该函数传入的node为为要插入的节点，下面判断node是否为叶子节点，如果不是叶子节点则继续递归该插入函数
    void internal_insert_value(BPlus_tree_node<key_type, value_type> *node, key_type key, value_type value);

    // 叶子节点分裂函数
    // 叶子节点分裂需要提一个key到父亲节点且保留该key在叶子节点中
    // 而索引节点分裂时提key但不需要保留key
    // 二者分裂有区别所以将二者分裂函数分开写
    void split_leaf_node(BPlus_tree_node<key_type, value_type> *leaf);

    // 非叶子节点分裂函数
    void split_index_node(BPlus_tree_node<key_type, value_type> *node);

    // 内部删除函数
    // 如果所在结点不是叶子节点，则需要递归往下搜索直到搜索到叶子节点
    // 找到叶子节点后才能进行删除操作
    bool internal_drop_value(key_type key);

    // 叶子节点合并函数
    void merge_leaf_into_left_sibling(BPlus_tree_node<key_type, value_type> *parent, int cur_pos, int left_pos);

    void merge_leaf_into_right_sibling(BPlus_tree_node<key_type, value_type> *parent, int cur_pos, int right_pos);

    // 叶子节点旋转函数
    void rotate_rightly_with_left_sibling(BPlus_tree_node<key_type, value_type> *parent, int cur_pos, int left_pos);

    void rotate_leftly_with_right_sibling(BPlus_tree_node<key_type, value_type> *parent, int cur_pos, int right_pos);

    // 索引节点整理函数（包含了合并和旋转）
    void index_balance(BPlus_tree_node<key_type, value_type> *node);

    // 索引节点合并函数
    void merge_index_into_left_sibling(BPlus_tree_node<key_type, value_type> *parent, int cur_pos, int left_pos);

    void merge_index_into_right_sibling(BPlus_tree_node<key_type, value_type> *parent, int cur_pos, int right_pos);

    // 索引节点旋转函数
    void
    index_rotate_rightly_with_left_sibling(BPlus_tree_node<key_type, value_type> *parent, int cur_pos, int left_pos);

    void
    index_rotate_leftly_with_right_sibling(BPlus_tree_node<key_type, value_type> *parent, int cur_pos, int right_pos);


};

//找到key对应的索引，没有找到则返回-1
template<typename key_type, typename value_type>
int find_key_index(const vector<key_type> &keys, key_type key)
{
    int left = 0;
    int right = keys.size() - 1;
    while (left <= right)
    {
        int mid = (left + right) / 2;
        if (keys[mid] == key)
        {
            return mid;
        } else if (key < keys[mid])
        {
            right = mid - 1;
        } else // keys[mid] < key
        {
            left = mid + 1;
        }
    }
    return -1;
}

//找到最靠右的且比key小的key右边那一格
template<typename key_type, typename value_type>
int find_key_pos(const vector<key_type> &keys, key_type key)
{
    int temp_index = find_key_index<key_type, value_type>(keys, key);
    if (temp_index != -1)
        return temp_index + 1;

    int left = 0;
    int right = keys.size();

    while (left < right)
    {
        int mid = (left + right) / 2;
        if (keys[mid] < key)
        {
            left = mid + 1;
        } else
        {
            right = mid;
        }
    }
    return left;
}

template<typename key_type, typename value_type>
vector<key_type> BPlus_tree<key_type, value_type>::search_range(key_type min_key, key_type max_key)
{
    // 找到第一个大于等于min_key的节点，cur_node记录该结点的位置
    BPlus_tree_node<key_type, value_type> *cur_node = root;
    while (!cur_node->is_leaf)
    {
        int pos = find_key_pos<key_type, value_type>(cur_node->keys, min_key);
        cur_node = cur_node->children[pos];
    }
    vector<value_type> vector_main_key;
    // 如果该节点是 nullptr 直接返回一个空vector_main_key
    while (cur_node != nullptr)
    {
        vector<key_type> &keys = cur_node->keys;
        vector<value_type> &values = cur_node->values;
        for (int i = 0; i < keys.size(); i++)
        {
            if (min_key <= keys[i] && max_key >= keys[i])
                vector_main_key.push_back(values[i]);
            else if (keys[i] > max_key)
                return vector_main_key;
        }
        cur_node = cur_node->next;
    }
    return vector_main_key;
}

template<typename key_type, typename value_type>
vector<key_type> BPlus_tree<key_type, value_type>::search_range(key_type key, bool up)
{
    // 找到第一个大于等于min_key的节点，cur_node记录该结点的位置
    BPlus_tree_node<key_type, value_type> *cur_node = root;
    while (!cur_node->is_leaf)
    {
        int pos = find_key_pos<key_type, value_type>(cur_node->keys, key);
        cur_node = cur_node->children[pos];
    }


    vector<value_type> vector_main_key;
    if (up)
    {
        while (cur_node != nullptr)
        {
            vector<key_type> &keys = cur_node->keys;
            vector<value_type> &values = cur_node->values;
            for (int i = 0; i < keys.size(); i++)
            {
                if (keys[i] >= key)
                    vector_main_key.push_back(values[i]);
            }
            cur_node = cur_node->next;
        }
        return vector_main_key;
    } else
    {
        while (cur_node != nullptr)
        {
            vector<key_type> &keys = cur_node->keys;
            vector<value_type> &values = cur_node->values;
            for (int i = 0; i < keys.size(); i++)
            {
                if (keys[i] <= key)
                    vector_main_key.push_back(values[i]);
            }
            cur_node = cur_node->left;
        }
        return vector_main_key;
    }
    // 如果该节点是 nullptr 直接返回一个空vector_main_key
    return vector_main_key;
}

template<typename key_type, typename value_type>
void BPlus_tree<key_type, value_type>::insert_value(key_type key, const value_type &value)
{
    if (root == nullptr)
    {
        BPlus_tree_node<key_type, value_type> *new_node = new BPlus_tree_node<key_type, value_type>(true);
        root = new_node;
        root->keys.push_back(key);
        root->values.push_back(value);
    } else
    {
        internal_insert_value(root, key, value);
    }
}

template<typename key_type, typename value_type>
void BPlus_tree<key_type, value_type>::internal_insert_value(BPlus_tree_node<key_type, value_type> *node, key_type key,
                                                             value_type value)
{
    if (node->is_leaf)
    {
        if (find_key_index<key_type, value_type>(node->keys, key) != -1)
        {
            cout << "This key already exist!" << endl;
            cout << "Please use update!" << endl;
            return;
        }

        //起别名，方便操作
        vector<key_type> &keys = node->keys;
        vector<value_type> &values = node->values;

        //找插入位置的索引，执行插入操作
        int pos = find_key_pos<key_type, value_type>(keys, key);
        keys.insert(keys.begin() + pos, key);
        values.insert(values.begin() + pos, value);

        if (keys.size() >= ORDER)
        {
            // split_leaf_node 会执行 split_index_node
            // split_index_node 会递归执行 split_index_node
            split_leaf_node(node);
        }
    }
        // 如果不是叶子节点，继续往下搜索
    else
    {
        vector<key_type> &keys = node->keys;
        int pos = find_key_pos<key_type, value_type>(keys, key);
        node = node->children[pos];
        internal_insert_value(node, key, value);
    }
}

template<typename key_type, typename value_type>
void BPlus_tree<key_type, value_type>::split_leaf_node(BPlus_tree_node<key_type, value_type> *leaf)
{
    BPlus_tree_node<key_type, value_type> *new_node = new BPlus_tree_node<key_type, value_type>(true);

    // key and value (从ORDER/2 到最后）转移
    for (int i = ORDER / 2; i < leaf->keys.size(); i++)
    {
        new_node->keys.push_back(leaf->keys[i]);
        new_node->values.push_back(leaf->values[i]);
    }

    // 对原结点执行erase操作
    leaf->keys.erase(leaf->keys.begin() + ORDER / 2, leaf->keys.end());
    leaf->values.erase(leaf->values.begin() + ORDER / 2, leaf->values.end());

    // 维护next和left
    new_node->next = leaf->next;
    if (leaf->next != nullptr)
    {
        leaf->next->left = new_node;
    }
    leaf->next = new_node;
    new_node->left = leaf;

    // 新建parent
    // 分为两种：有父亲节点/无父亲节点
    // 需要维护parent
    if (leaf->parent == nullptr)
    {
        BPlus_tree_node<key_type, value_type> *parent = new BPlus_tree_node<key_type, value_type>(false);
        parent->keys.push_back(new_node->keys[0]);
        parent->children.push_back(leaf);
        parent->children.push_back(new_node);

        leaf->parent = parent;
        new_node->parent = parent;

        root = parent;
    }
        // 如果叶子结点分裂时已经有了parent
    else if (leaf->parent != nullptr)
    {
        BPlus_tree_node<key_type, value_type> *parent = leaf->parent;
        key_type key = new_node->keys[0];
        int pos = find_key_pos<key_type, value_type>(parent->keys, key);

        parent->keys.insert(parent->keys.begin() + pos, key);
        parent->children.insert(parent->children.begin() + pos + 1, new_node);

        new_node->parent = parent;

        if (parent->keys.size() >= ORDER)
        {
            split_index_node(parent);
        }
    }
}

template<typename key_type, typename value_type>
void BPlus_tree<key_type, value_type>::split_index_node(BPlus_tree_node<key_type, value_type> *index_node)
{
    // 新建new_node，作为index_node的邻节点
    BPlus_tree_node<key_type, value_type> *new_node = new BPlus_tree_node<key_type, value_type>(false);
    key_type up_insert_key = index_node->keys[ORDER / 2];

    // key and children转移到新节点
    // 同时要维护 children 的 parent节点
    for (int i = ORDER / 2 + 1; i < index_node->keys.size(); i++)
    {
        key_type temp_key = index_node->keys[i];
        new_node->keys.push_back(temp_key);
    }
    for (int i = ORDER / 2 + 1; i < index_node->children.size(); i++)
    {
        BPlus_tree_node<key_type, value_type> *temp_child = index_node->children[i];
        new_node->children.push_back(temp_child);
        temp_child->parent = new_node;
    }

    // 对原结点执行erase操作
    index_node->keys.erase(index_node->keys.begin() + ORDER / 2, index_node->keys.end());
    index_node->children.erase(index_node->children.begin() + ORDER / 2 + 1, index_node->children.end());

    // 此部分将up_insert_key插入parent节点，维护root，维护parent节点
    if (index_node->parent == nullptr)
    {
        // 新建parent节点，传入key和children数据
        BPlus_tree_node<key_type, value_type> *parent = new BPlus_tree_node<key_type, value_type>(false);
        parent->keys.push_back(up_insert_key);
        parent->children.push_back(index_node);
        parent->children.push_back(new_node);

        index_node->parent = parent;
        new_node->parent = parent;

        // 此处的if判断一定成立
        if (root == index_node)
            root = parent;
    } else if (index_node->parent != nullptr)
    {
        // 创建新名字，提高可读性
        BPlus_tree_node<key_type, value_type> *parent = index_node->parent;

        // 找到key插入的位置并插入该key
        int pos = find_key_pos<key_type, value_type>(parent->keys, up_insert_key);
        parent->keys.insert(parent->keys.begin() + pos, up_insert_key);

        // 维护new_node的parent和parent的children
        parent->children.insert(parent->children.begin() + pos + 1, new_node);
        new_node->parent = parent;

        // 在split>index_node函数中递归执行split_index_node
        if (parent->keys.size() >= ORDER)
        {
            split_index_node(parent);
        }
    }
}

template<typename key_type, typename value_type>
BPlus_tree_node<key_type, value_type> *BPlus_tree<key_type, value_type>::find_key_node(key_type key)
{
    BPlus_tree_node<key_type, value_type> *cur_node = root;
    while (cur_node != nullptr)
    {
        // 如果当前节点是叶子结点
        if (cur_node->is_leaf)
        {
            int index = find_key_index<key_type, value_type>(cur_node->keys, key);
            // 没找到则返回空指针
            if (index == -1)
                return nullptr;
                // 找到了则返回当前叶子节点
            else
                return cur_node;
        }
            // 如果不是叶子结点，则需要继续往下搜索
        else
        {
            int pos = find_key_pos<key_type, value_type>(cur_node->keys, key);
            cur_node = cur_node->children[pos];
        }
    }
    // 只有当cur_node == nullptr才会执行到这句
    return nullptr;
}

template<typename key_type, typename value_type>
value_type BPlus_tree<key_type, value_type>::search_value(key_type key)
{
    BPlus_tree_node<key_type, value_type> *node = find_key_node(key);
    if (node == nullptr)
    {
        // 如果没有找到，输出并返回NULL
        // cout << "Not found!" << endl;
        return -1;
    }
    else
    {
        int index = find_key_index<key_type, value_type>(node->keys, key);
        return node->values[index];
    }
}

template<typename key_type, typename value_type>
bool BPlus_tree<key_type, value_type>::update(key_type key, value_type value)
{
    BPlus_tree_node<key_type, value_type> *node = find_key_node(key);
    if (node == nullptr)
    {
        // 如果没有找到，输出并返回false
        // cout << "Not found!" << endl;
        return false;
    } else
    {
        int index = find_key_index<key_type, value_type>(node->keys, key);
        node->values[index] = value;
        return true;
    }
}

template<typename key_type, typename value_type>
void BPlus_tree<key_type, value_type>::print()
{
    // 如果该树没有值，则不输出
    if (root == nullptr)
        return;

    // 找到存储最小值的最小叶子节点（左下方）
    // 按key升序输出所有的value
    BPlus_tree_node<key_type, value_type> *cur_node = root;
    while (!cur_node->is_leaf)
    {
        cur_node = cur_node->children[0];
    }

    // 开始往右遍历（通过next）
    static int count = 1;
    while (cur_node != nullptr)
    {
        cout << "Leaf_Node" << "(" << count << "): ";
        for (int i = 0; i < cur_node->values.size(); i++)
        {
            cout << cur_node->values[i] << " ";
        }
        cout << endl;

        // 继续循环输出
        count++;
        cur_node = cur_node->next;
    }
    count = 1;
}

template<typename key_type, typename value_type>
bool BPlus_tree<key_type, value_type>::if_key_exist(key_type key)
{
    BPlus_tree_node<key_type, value_type> *node = find_key_node(key);
    if (node == nullptr)
    {
        return false;
    } else
    {
        return true;
    }
}

// 删除函数
template<typename key_type, typename value_type>
bool BPlus_tree<key_type, value_type>::drop_value(key_type key)
{
    if (root == nullptr)
        return false;

    else
        return internal_drop_value(key);
}

template<typename key_type, typename value_type>
bool BPlus_tree<key_type, value_type>::internal_drop_value(key_type key)
{
    // 调用找叶子节点函数
    BPlus_tree_node<key_type, value_type> *node = find_key_node(key);

    // 没找到，则直接返回删除失败
    if (node == nullptr)
        return false;

    // 能执行到这里说明找到了
    // 删除对应key和value
    int key_index = find_key_index<key_type, value_type>(node->keys, key);
    node->keys.erase(node->keys.begin() + key_index);
    node->values.erase(node->values.begin() + key_index);

    // 如果仅有当前节点作为根节点，直接退出
    // 因为根节点不需要执行合并和旋转操作
    if (node->parent == nullptr)
        return true;

    // 如果删除后当前节点key过少，需要和兄弟节点执行合并/旋转
    // 优先执行合并，因为合并可以减少节点数量提高B+树检索效率
    if (node->keys.size() < NODE_MINIMUM_DEGREE)
    {
        // 如果当前节点的元素个数小于NODE_MINIMUM_DEGREE
        // 执行合并操作，前提是它的相邻兄弟节点拥有足够的空位
        // 否则需要执行旋转操作

        // 在父节点中找到左兄弟节点和右兄弟节点并命名
        int cur_pos = find_key_pos<key_type, value_type>(node->parent->keys, node->keys[0]);
        int left_pos = cur_pos - 1;  // 左兄弟节点在parent的children中的位置（可能小于0）
        int right_pos = cur_pos + 1; // 右兄弟节点在parent的children中的位置（可能大于ORDER）
        BPlus_tree_node<key_type, value_type> *left_sibling = nullptr;
        BPlus_tree_node<key_type, value_type> *right_sibling = nullptr;
        if (left_pos >= 0)
            left_sibling = node->parent->children[left_pos];
        if (right_pos <= ORDER)
            right_sibling = node->parent->children[right_pos];

        // 后面的条件代表着兄弟节点有足够的空位容纳该节点的数据
        // leaf_node中被合并的key数量为 NODE_MINIMUM_DEGREE - 1
        // 且parent中没有key需要下移
        // 此处每次操作直接return，不会重复合并/旋转
        // 合并操作
        if (left_sibling != nullptr)
        {
            // 与左兄弟节点合并
            if (left_sibling->keys.size() + NODE_MINIMUM_DEGREE - 1 < ORDER)
            {
                merge_leaf_into_left_sibling(node->parent, cur_pos, left_pos);
                return true;
            }
        }
        if (right_sibling != nullptr)
        {
            // 与右兄弟节点合并
            if (right_sibling->keys.size() + NODE_MINIMUM_DEGREE - 1 < ORDER)
            {
                merge_leaf_into_right_sibling(node->parent, cur_pos, right_pos);
                return true;
            }
        }

        //如果无法合并，则执行旋转操作
        if (left_sibling != nullptr)
        {
            // 尝试与左兄弟节点旋转
            if (left_sibling->keys.size() > NODE_MINIMUM_DEGREE)
            {
                rotate_rightly_with_left_sibling(node->parent, cur_pos, left_pos);
                return true;
            }
        }
        if (right_sibling != nullptr)
        {
            // 尝试与右兄弟节点旋转
            if (right_sibling->keys.size() > NODE_MINIMUM_DEGREE)
            {
                rotate_leftly_with_right_sibling(node->parent, cur_pos, right_pos);
                return true;
            }
        }
    }
    return true;
}

template<typename key_type, typename value_type>
void BPlus_tree<key_type, value_type>::index_balance(BPlus_tree_node<key_type, value_type> *node)
{
    // 在父节点中找到左兄弟节点和右兄弟节点并命名
    int cur_pos = find_key_pos<key_type, value_type>(node->parent->keys, node->keys[0]);
    int left_pos = cur_pos - 1;  // 左兄弟节点在parent的children中的位置（可能小于0）
    int right_pos = cur_pos + 1; // 右兄弟节点在parent的children中的位置（可能大于ORDER）
    BPlus_tree_node<key_type, value_type> *left_sibling = nullptr;
    BPlus_tree_node<key_type, value_type> *right_sibling = nullptr;
    if (left_pos >= 0)
        left_sibling = node->parent->children[left_pos];
    if (right_pos <= ORDER)
        right_sibling = node->parent->children[right_pos];

    // 后面的条件代表着兄弟节点有足够的空位容纳该节点的数据
    // leaf_node中被合并的key数量为 NODE_MINIMUM_DEGREE - 1
    // 且parent中没有key需要下移
    // 此处每次操作直接return，不会重复合并/旋转

    // 往左合并
    if (left_sibling != nullptr)
    {
        if (left_sibling->keys.size() + NODE_MINIMUM_DEGREE < ORDER)
        {
            merge_index_into_left_sibling(node->parent, cur_pos, left_pos);
            return;
        }
    }
    // 往右合并
    if (right_sibling != nullptr)
    {
        // 与右兄弟节点合并
        if (right_sibling->keys.size() + NODE_MINIMUM_DEGREE < ORDER)
        {
            merge_index_into_right_sibling(node->parent, cur_pos, right_pos);
            return;
        }
    }

    // 如果无法合并，则执行旋转操作
    // 与左兄弟节点右旋转
    if (left_sibling != nullptr)
    {
        // 尝试与左兄弟节点旋转
        if (left_sibling->keys.size() > NODE_MINIMUM_DEGREE)
        {
            index_rotate_rightly_with_left_sibling(node->parent, cur_pos, left_pos);
            return;
        }
    }
    // 与右兄弟节点左旋转
    if (right_sibling != nullptr)
    {
        // 尝试与右兄弟节点旋转
        if (right_sibling->keys.size() > NODE_MINIMUM_DEGREE)
        {
            index_rotate_leftly_with_right_sibling(node->parent, cur_pos, right_pos);
            return;
        }
    }
}

// 叶子节点合并函数
template<typename key_type, typename value_type>
void BPlus_tree<key_type, value_type>::merge_leaf_into_left_sibling(BPlus_tree_node<key_type, value_type> *parent,
                                                                    int cur_pos, int left_pos)
{
    BPlus_tree_node<key_type, value_type> *left_sibling = parent->children[left_pos];
    BPlus_tree_node<key_type, value_type> *cur_node = parent->children[cur_pos];

    // 将cur_node的数据转移到left_node
    for (int i = 0; i < cur_node->keys.size(); i++)
    {
        left_sibling->keys.push_back(cur_node->keys[i]);
        left_sibling->values.push_back(cur_node->values[i]);
    }

    // 更新next和left指针
    if (cur_node->next != nullptr)
    {
        cur_node->next->left = left_sibling;
        left_sibling->next = cur_node->next;
    } else
    {
        left_sibling->next = nullptr;
    }

    // 删除节点
    cur_node->clear();
    delete cur_node;

    // 更新parent节点的key、children
    parent->keys.erase(parent->keys.begin() + left_pos);
    parent->children.erase(parent->children.begin() + cur_pos);

    // 更新root
    if (parent->keys.empty())
    {
        parent->clear();
        delete parent;
        left_sibling->parent = nullptr;
        root = left_sibling;
    }

        // 递归更新
    else if (parent->keys.size() < NODE_MINIMUM_DEGREE)
    {
        index_balance(parent);
    }
}

template<typename key_type, typename value_type>
void BPlus_tree<key_type, value_type>::merge_leaf_into_right_sibling(BPlus_tree_node<key_type, value_type> *parent,
                                                                     int cur_pos, int right_pos)
{
    BPlus_tree_node<key_type, value_type> *right_sibling = parent->children[right_pos];
    BPlus_tree_node<key_type, value_type> *cur_node = parent->children[cur_pos];

    // 将cur_node的数据转移到right_node
    for (int i = cur_node->keys.size() - 1; i >= 0; i--)
    {
        right_sibling->keys.insert(right_sibling->keys.begin(), cur_node->keys[i]);
        right_sibling->values.insert(right_sibling->values.begin(), cur_node->values[i]);
    }

    // 更新next和left指针
    if (cur_node->left != nullptr)
    {
        cur_node->left->next = right_sibling;
        right_sibling->left = cur_node->left;
    } else
    {
        right_sibling->left = nullptr;
    }

    // 删除节点
    cur_node->clear();
    delete cur_node;

    // 更新parent节点的key、children
    parent->keys.erase(parent->keys.begin() + cur_pos);
    parent->children.erase(parent->children.begin() + cur_pos);

    // 更新root
    if (parent->keys.empty())
    {
        parent->clear();
        delete parent;
        right_sibling->parent = nullptr;
        root = right_sibling;
    }

        // 递归更新
    else if (parent->keys.size() < NODE_MINIMUM_DEGREE)
    {
        index_balance(parent);
    }
}

//叶子节点旋转函数
template<typename key_type, typename value_type>
void BPlus_tree<key_type, value_type>::rotate_rightly_with_left_sibling(BPlus_tree_node<key_type, value_type> *parent,
                                                                        int cur_pos, int left_pos)
{
    BPlus_tree_node<key_type, value_type> *left_sibling = parent->children[left_pos];
    BPlus_tree_node<key_type, value_type> *cur_node = parent->children[cur_pos];

    //初始化一个temp_key，为左兄弟节点最右边的key，插入到cur_node，修改parent
    key_type temp_key = left_sibling->keys[left_sibling->keys.size() - 1];
    cur_node->keys.insert(cur_node->keys.begin(), temp_key);
    parent->keys[left_pos] = temp_key;

    // 下面两行插入value到cur_node
    // string temp_value = left_sibling->values[left_sibling->values.size() - 1];
    cur_node->values.insert(cur_node->values.begin(), left_sibling->values[left_sibling->values.size() - 1]);

    //删除left_sibling的temp数据
    left_sibling->keys.pop_back();
    left_sibling->values.pop_back();


    //没有发生节点的生成和删除，所以不需要变动指针指向
}

template<typename key_type, typename value_type>
void BPlus_tree<key_type, value_type>::rotate_leftly_with_right_sibling(BPlus_tree_node<key_type, value_type> *parent,
                                                                        int cur_pos, int right_pos)
{
    BPlus_tree_node<key_type, value_type> *right_sibling = parent->children[right_pos];
    BPlus_tree_node<key_type, value_type> *cur_node = parent->children[cur_pos];

    // 将right_sibling的最小key以及对应的value转移给cur_node
    cur_node->keys.push_back(right_sibling->keys[0]);
    cur_node->values.push_back(right_sibling->values[0]);

    // 删除right_sibling对应的数据
    right_sibling->keys.erase(right_sibling->keys.begin());
    right_sibling->values.erase(right_sibling->values.begin());

    // 更新parent的key
    // 此时索引为0的key是大于转移给cur_node的key的
    parent->keys.erase(parent->keys.begin() + cur_pos);
    parent->keys.insert(parent->keys.begin() + cur_pos, right_sibling->keys[0]);

    //没有发生节点的生成和删除，所以不需要变动指针指向
}

//索引节点合并函数
template<typename key_type, typename value_type>
void BPlus_tree<key_type, value_type>::merge_index_into_left_sibling(BPlus_tree_node<key_type, value_type> *parent,
                                                                     int cur_pos, int left_pos)
{
    BPlus_tree_node<key_type, value_type> *left_sibling = parent->children[left_pos];
    BPlus_tree_node<key_type, value_type> *cur_node = parent->children[cur_pos];

    //把parent的key下移
    left_sibling->keys.push_back(parent->keys[left_pos]);

    // 将cur_node的数据转移到left_node
    for (int i = 0; i < cur_node->keys.size(); i++)
    {
        left_sibling->keys.push_back(cur_node->keys[i]);
    }
    for (int i = 0; i < cur_node->children.size(); i++)
    {
        left_sibling->children.push_back(cur_node->children[i]);
        cur_node->children[i]->parent = left_sibling;
    }

    // 删除cur_node
    cur_node->clear();
    delete cur_node;

    //删除parent的一个key和一个child
    parent->keys.erase(parent->keys.begin() + left_pos);
    parent->children.erase(parent->children.begin() + cur_pos);

    if (parent->keys.empty())
    {
        parent->clear();
        delete parent;
        left_sibling->parent = nullptr;
        root = left_sibling;
    }

        // 递归更新
    else if (parent->keys.size() < NODE_MINIMUM_DEGREE)
    {
        index_balance(parent);
    }
}

template<typename key_type, typename value_type>
void BPlus_tree<key_type, value_type>::merge_index_into_right_sibling(BPlus_tree_node<key_type, value_type> *parent,
                                                                      int cur_pos, int right_pos)
{
    BPlus_tree_node<key_type, value_type> *right_sibling = parent->children[right_pos];
    BPlus_tree_node<key_type, value_type> *cur_node = parent->children[cur_pos];

    //把parent的key下移
    right_sibling->keys.insert(right_sibling->keys.begin(), parent->keys[cur_pos]);

    // 将cur_node的数据转移到left_node
    for (int i = cur_node->keys.size() - 1; i >= 0; i--)
    {
        right_sibling->keys.insert(right_sibling->keys.begin(), cur_node->keys[i]);
    }

    for (int i = cur_node->children.size() - 1; i >= 0; i--)
    {
        right_sibling->children.insert(right_sibling->children.begin(), cur_node->children[i]);
    }

    // 删除cur_node
    cur_node->clear();
    delete cur_node;

    //删除parent的一个key和一个child
    parent->keys.erase(parent->keys.begin() + cur_pos);
    parent->children.erase(parent->children.begin() + cur_pos);

    if (parent->keys.empty())
    {
        parent->clear();
        delete parent;
        right_sibling->parent = nullptr;
        root = right_sibling;
    }

        // 递归更新
    else if (parent->keys.size() < NODE_MINIMUM_DEGREE)
    {
        index_balance(parent);
    }
}

//索引节点旋转函数
template<typename key_type, typename value_type>
void
BPlus_tree<key_type, value_type>::index_rotate_rightly_with_left_sibling(BPlus_tree_node<key_type, value_type> *parent,
                                                                         int cur_pos, int left_pos)
{
    BPlus_tree_node<key_type, value_type> *left_sibling = parent->children[left_pos];
    BPlus_tree_node<key_type, value_type> *cur_node = parent->children[cur_pos];

    // 更新key
    // parent的key转移到cur_node
    // left_sibling的key转移到parent
    cur_node->keys.insert(cur_node->keys.begin(), parent->keys[left_pos]);
    parent->keys[left_pos] = left_sibling->keys[left_sibling->keys.size() - 1];
    left_sibling->keys.pop_back();

    // 更新children和children的parent
    // 把left_sibling的最右边一个指针转移到cur_node的第一个位置
    int temp_index = left_sibling->children.size() - 1;
    cur_node->children.insert(cur_node->children.begin(), left_sibling->children[temp_index]);
    left_sibling->children[temp_index]->parent = cur_node;
    left_sibling->children.pop_back();
}

template<typename key_type, typename value_type>
void
BPlus_tree<key_type, value_type>::index_rotate_leftly_with_right_sibling(BPlus_tree_node<key_type, value_type> *parent,
                                                                         int cur_pos, int right_pos)
{
    BPlus_tree_node<key_type, value_type> *right_sibling = parent->children[right_pos];
    BPlus_tree_node<key_type, value_type> *cur_node = parent->children[cur_pos];

    // 更新key
    // parent的key转移到cur_node
    // right_sibling的key转移到parent
    cur_node->keys.push_back(parent->keys[cur_pos]);
    parent->keys[cur_pos] = right_sibling->keys[0];
    right_sibling->keys.erase(right_sibling->keys.begin());

    // 更新children和children的parent
    // 把right_sibling的最左边一个指针转移到cur_node的最右边
    cur_node->children.push_back(right_sibling->children[0]);
    right_sibling->children[0]->parent = cur_node;
    right_sibling->children.pop_back();
}

void show_catalog()
{
    cout << "-----------------------------------------------------" << endl;
    cout << "(1)void insert_value(key_type key, value_type value){...}" << endl;
    cout << "(2)value_type search_value(key_type key){...}" << endl;
    cout << "(3)bool drop_value(key_type key){...}" << endl;
    cout << "(4)bool update(key_type key, value_type value){...}" << endl;
    cout << "(5)void Print(){...}" << endl;
    cout << "(6)key_type search_key(value_type value){...}" << endl;    // 还没实现
    cout << "(7)bool if_key_exist(key_type key){...}" << endl;
    cout << "-----------------------------------------------------" << endl;
}

#endif // BIN_BPLUS_TREE_H
