#include <cassert>
#include <iomanip>
#include <iostream>
#include <stdlib.h>

template <class Item> class bag {
  public:
    bag();
    bag(bag &source);
    ~bag();

    size_t count(const Item &target) const;
    void insert(const Item &entry);
    bool erase_one(const Item &target);
    void show_contents();
    void clear();
    template <class Item_> friend bag<Item_> *bag_copy(bag<Item_> *source);

  private:
    static const size_t MINIMUM = 1;
    static const size_t MAXIMUM = 2 * MINIMUM;
    size_t data_count;
    Item data[MAXIMUM + 1];
    size_t child_count;
    bag *child[MAXIMUM + 2];

    void loose_insert(const Item &entry);
    void fix_excess(size_t i);

    bool loose_erase(const Item &target);
    void fix_shortage(size_t i);
    void remove_biggest(Item &removed_enrty);

    void show_contents_rec(size_t depth);

    void insert_data(size_t i, const Item &entry);
    void insert_child(size_t i, bag *child);
    void pull_data(size_t i);
    void pull_child(size_t i);
};

template <class Item> bag<Item>::bag() {
    data_count = 0;
    child_count = 0;
    for (size_t i = 0; i < MAXIMUM + 2; i++)
        child[i] = NULL;
}

template <class Item> bag<Item>::bag(bag &source) {
    size_t i;
    bag *root_ptr = bag_copy(&source);
    data_count = root_ptr->data_count;
    child_count = root_ptr->child_count;

    for (i = 0; i < data_count; i++) {
        data[i] = root_ptr->data[i];
        child[i] = root_ptr->child[i];
    }
    child[i] = root_ptr->child[i];
}

template <class Item> bag<Item>::~bag() { clear(); }

template <class Item> size_t bag<Item>::count(const Item &target) const {
    int count = 0;
    int index = 0;

    while (data[index] < target && index < data_count) {
        ++index; // target이 있을만한 data나 child의 index를 찾는다
    }

    if (index < data_count && data[index] == target) {
        /* target이 같은 노드나 왼쪽, 오른쪽 자식에 또 존재할수 있기 때문에 모두
         * 찾는다 */
        while (data[index] == target && index < data_count) {
            ++count;

            // 자식이 있는경우 target의 왼쪽 자식에서 찾는다
            if (child_count != 0)
                count += child[index]->count(target);

            ++index;
        }

        if (child_count != 0)
            count += child[index]->count(target); // 마지막 오른쪽 자식도 확인
    } else if (child_count != 0) {
        // target이 해당 노드에 없는데 자식이 존재하므로 자식에 있는지 찾는다
        return child[index]->count(target);
    }

    // 자식이 없는 리프노드에서 target이 발견되지 않는경우 위의 if문을 모두
    // 만족시키지 못하고 count의 값에는 0이 저장된다. 따라서 이 경우에는 그냥
    // count를 반환하면 된다

    return count;
}

template <class Item> void bag<Item>::insert(const Item &entry) {
    loose_insert(entry);

    if (data_count == MAXIMUM + 1) {
        bag *copy_this = new bag(*this);
        clear();
        insert_child(0, copy_this);
        fix_excess(0);
    }
}

template <class Item> void bag<Item>::loose_insert(const Item &entry) {
    size_t index = 0;

    while (data[index] < entry && index < data_count) {
        ++index; // entry를 넣을 만한 data나 child의 index를 찾는다
    }

    if (child_count != 0) { // 리프노드에 도달할때까지 재귀적으로 호출
        child[index]->loose_insert(entry);

        /* 재귀 호출 후 child가 MAXIMUM 조건을 만족시키지 못하면 fix */
        if (child[index]->data_count == MAXIMUM + 1)
            fix_excess(index);
    } else { // 리프노드라면 entry를 삽입
        insert_data(index, entry);
    }
}

template <class Item> void bag<Item>::fix_excess(size_t i) {
    assert(i >= 0 && i < child_count);
    assert(child[i]->data_count == MAXIMUM + 1);

    bag *splited_child = new bag(); // 분리시킬 자식 서브트리

    /* 부모에 중간 데이터 삽입 */
    insert_data(i, child[i]->data[MINIMUM]);

    /* 자식의 데이터 반을 분리 */
    for (int j = 0; j < MINIMUM; j++) {
        splited_child->data[j] = child[i]->data[MINIMUM + 1 + j];
    }
    splited_child->data_count = MINIMUM;
    child[i]->data_count = MINIMUM;

    /* 자식이 자식을 가진다면 자식의 자식을 분리 */
    if (child[i]->child_count != 0) {
        for (int j = 0; j <= MINIMUM; j++) {
            /* shallow copy 후 분리된 자식은 모두 NULL로 할당 */
            splited_child->child[j] = child[i]->child[MINIMUM + 1 + j];
            child[i]->child[MINIMUM + 1 + j] = NULL;
        }
        splited_child->child_count = MINIMUM + 1;
        child[i]->child_count = MINIMUM + 1;
    }

    insert_child(i + 1, splited_child); // 분리된 자식을 삽입
}

template <class Item> bool bag<Item>::erase_one(const Item &target) {}

template <class Item> bool bag<Item>::loose_erase(const Item &target) {}

template <class Item> void bag<Item>::fix_shortage(size_t i) {}

template <class Item> void bag<Item>::remove_biggest(Item &removed_enrty) {}

template <class Item> void bag<Item>::show_contents() { show_contents_rec(0); }
template <class Item> void bag<Item>::show_contents_rec(size_t depth) {
    int i;
    if (child_count != 0) {
        for (i = child_count - 1; i > 0; i--) {
            child[i]->show_contents_rec(depth + 1);
            std::cout << std::setw(4 * depth) << "" << data[i - 1] << std::endl;
        }
        child[i]->show_contents_rec(depth + 1);
    } else {
        for (i = data_count - 1; i >= 0; i--) {
            std::cout << std::setw(4 * depth) << "" << data[i] << std::endl;
        }
    }
}

template <class Item> void bag<Item>::insert_data(size_t i, const Item &entry) {
    assert(i <= data_count);
    for (size_t j = data_count; j > i; j--)
        data[j] = data[j - 1];

    data[i] = entry;
    data_count++;
}

template <class Item> void bag<Item>::insert_child(size_t i, bag *child) {
    assert(i <= child_count);
    for (size_t j = child_count; j > i; j--)
        this->child[j] = this->child[j - 1];

    this->child[i] = child;
    child_count++;
}

template <class Item> void bag<Item>::pull_data(size_t i) {}

template <class Item> void bag<Item>::pull_child(size_t i) {}

template <class Item> void bag<Item>::clear() {
    if (child_count != 0) {
        for (size_t i = 0; i < child_count; i++) {
            child[i]->clear();
            delete child[i];
            child[i] = NULL;
        }
        child_count = 0;
    }
    data_count = 0;
}

template <class Item> bag<Item> *bag_copy(bag<Item> *source) {
    if (source == NULL)
        return NULL;

    bag<Item> *copy_node = new bag<Item>();

    for (size_t i = 0; i < source->data_count; i++) {
        copy_node->data[i] = source->data[i];
    }
    copy_node->data_count = source->data_count;

    for (size_t i = 0; i < source->child_count; i++) {
        bag<Item> *copy_child = new bag<Item>();
        copy_child = bag_copy(source->child[i]);
        copy_node->child[i] = copy_child;
    }
    copy_node->child_count = source->child_count;

    return copy_node;
}
