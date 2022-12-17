#include <cassert>
#include <iomanip>
#include <iostream>
#include <stdlib.h>

/* bag을 B-tree로 구현한 template class 입니다. bag이기 때문에 같은 데이터를
 * 중복해서 저장할수 있습니다. */

template <class Item> class bag {
  public:
    // Default Constructor
    bag();

    // Copy Constructor
    bag(bag &source);

    // Destructor
    ~bag();

    // count() 함수는 현재 bag에 target이 몇개 존재하는지 반환하는 함수입니다.
    // Pre : None.
    // Post : bag에 있는 target의 개수를 반환. 없을 시 0 반환.
    size_t count(const Item &target) const;

    // insert() 함수는 현재 bag에 entry를 삽입하는 함수입니다.
    // Pre : None.
    // Post : bag에 enrty가 추가된다.
    void insert(const Item &entry);

    // erase_one 함수는 현재 bag에서 target 하나를 제거하는 함수입니다.
    // Pre : None.
    // Post : bag에 taget이 존재하면 삭제후 true 반환.
    //        target이 존재하지 않으면 false 반환.
    bool erase_one(const Item &target);

    // show_contents() 함수는 bag을 구현하고 있는 B-tree의 현재 상태를 가로로
    // 출력하는 함수입니다.
    // Pre : None.
    // Post : 화면에 B-tree가 가로로 출력된다.
    void show_contents();

    // clear() 함수는 현재 bag을 빈 bag으로 만드는 함수입니다.
    // Pre : None.
    // Post : bag에 동적 할당된 모든 자원을 free 시킨 후 bag을 비운다.
    void clear();

    // bag_copy() 함수는 bag인 source를 복사해서 반환하는 함수입니다.
    // Pre : None.
    // Post : source를 deep copy 후 복사된 bag의 포인터를 반환.
    template <class Item_> friend bag<Item_> *bag_copy(bag<Item_> *source);

  private:
    static const size_t MINIMUM = 1;
    static const size_t MAXIMUM = 2 * MINIMUM;
    size_t data_count; // 현재 노드에 저장하고 있는 데이터의 수
    Item data[MAXIMUM + 1];
    size_t child_count; // 현재 노드의 자식 수
    bag *child[MAXIMUM + 2];

    // loose_insert()는 현재 bag에 entry를 추가하되, MAXIMUM 조건을 깨뜨릴
    // 가능성이 있습니다. 즉 삽입 후 특정 노드의 data 수가 MAXIMUM + 1 개가 될수
    // 있습니다.
    // Pre : None.
    // Post : 현재 bag에 entry를 삽입. data의 수가 MAXIMUM + 1 개가 될수 있음.
    void loose_insert(const Item &entry);

    // fix_excess()는 loose_insert()후 child[i]의 data 수가 MAXIMUM + 1 개가
    // 될때 이를 처리하는 함수입니다.
    // Pre : (i < child_count) && (child[i]->data_count == MAXIMUM + 1)
    // Post : child[i]의 가운데 데이터를 현재노드에 추가하고 child[i]를 두개의
    //        child로 분할.
    void fix_excess(size_t i);

    // loose_erase()는 현재 bag에서 target을 제거하되, MINIMUM 조건을 깨뜨릴
    // 가능성이 있습니다. 즉 삽입 후 특정 노드의 data 수가 MINIMUM - 1 개가 될수
    // 있습니다.
    // Pre : None.
    // Post : 현재 bag에 target을 제거. data의 수가 MINIMUM - 1 개가 될수 있음.
    //        루트 노드의 경우 data의 수가 0개가 될수 있음.
    void loose_erase(const Item &target);

    // fix_shortage()는 loose_erase()후 child[i]의 data 수가 MINIMUM - 1 개가
    // 될때 이를 처리하는 함수입니다.
    // Pre : (i < child_count) && (child[i]->data_count == MINIMUM - 1)
    // Post : child[i-1]이나 child[i+1]에서 데이터를 가져올수 있으면 child[i]에
    //        가져온다. 데이터를 가져올수 없다면 현재 노드에서 데이터를 하나
    //        빼서 child[i]와 child[i-1](또는 child[i+1])를 merge한다.
    void fix_shortage(size_t i);

    // remove_biggest()는 현재 bag에서 가장 큰 데이터를 removed_enrty에 저장후
    // 삭제하는 함수입니다.
    // Pre : None.
    // Post : 현재 노드를 루트노드로 하는 bag에서 가장 큰 데이터를
    //        removed_entry에 저장후 삭제한다. 노드를 삭제하면서 data의 수가
    //        MINIMUM - 1개가 될수 있음.
    void remove_biggest(Item &removed_enrty);

    // show_contents_rec()은 show_contents()에서 재귀적으로 호출하는 함수입니다.
    // 현재 B-tree를 화면에 가로로 출력합니다.
    // Pre : None.
    // Post : 현재 B-tree를 화면에 가로로 출력.
    void show_contents_rec(size_t depth);

    // insert_data()는 data[i]에 enrty를 삽입하는 함수입니다.
    // Pre : i <= data_count
    // Post : data[i]에 enrty를 삽입. data[i]에 이미 아이템이 존재한다면 한칸씩
    //        뒤로 민다. data_count는 1 증가.
    void insert_data(size_t i, const Item &entry);

    // insert_child()는 child[i]에 child를 삽입하는 함수입니다.
    // Pre : i <= child_count
    // Post : child[i]에 child를 삽입. child[i]에 이미 아이템이 존재한다면
    // 한칸씩 뒤로 민다. child_count는 1 증가.
    void insert_child(size_t i, bag *child);

    // remove_data()는 data[i]를 삭제하는 함수입니다.
    // Pre : i < data_count
    // Post : data[i]를 삭제. data[i+1]에 아이템이 존재한다면 한칸씩 앞으로
    //        당긴다. data_count는 1 감소.
    void remove_data(size_t i);

    // remove_child()는 child[i]를 삭제하는 함수입니다.
    // Pre : i < child_count
    // Post : child[i]를 삭제. child[i+1]에 아이템이 존재한다면 한칸씩 앞으로
    //        당긴다. child_count는 1 감소.
    void remove_child(size_t i);

    // merge_child()는 fix_shortage()에서 두 child를 merge할때 사용하는
    // 함수입니다.
    // Pre : i < child_count - 1
    // Post : child[i]와 child[i+1]을 merge. 이때 현재 노드의 데이터 하나를
    //        가져와 child[i]와 child[i+1] 사이에 둔다.
    void merge_child(size_t i);
};

template <class Item> bag<Item>::bag() {
    data_count = 0;
    child_count = 0;
    for (size_t i = 0; i < MAXIMUM + 2; i++)
        child[i] = NULL;
}

template <class Item> bag<Item>::bag(bag &source) {
    size_t i;

    data_count = source.data_count;
    for (i = 0; i < data_count; i++) {
        data[i] = source.data[i];
    }

    child_count = source.child_count;
    for (i = 0; i < child_count; i++) {
        // 자식들을 모두 deep_copy
        child[i] = bag_copy(source.child[i]);
    }
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
        // 최상위 노드가 MAXIMUM 조건을 만족시키지 못하므로 데이터를 부모로
        // 올려야하는데, 최상위 노드는 부모노드가 존재하지 않는다. 따라서 현재
        // 노드를 deep copy 후 현재 노드를 빈 bag으로 만든뒤 복사한 노드를
        // 자식으로 삼는다.
        bag *copy_this = new bag(*this); // 자신을 copy_this에 deep copy
        clear();                         // 자신을 clear
        insert_child(0, copy_this); // copy_this를 첫번째 자식으로 삼는다.
        fix_excess(0); // child[0], 즉 copy_this가 MAXIMUM 조건을 만족시키지
                       // 못하므로 fix_excess(0)를 호출한다.
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
    assert(i < child_count);                     // Precondition
    assert(child[i]->data_count == MAXIMUM + 1); // Precondition

    bag *splited_child = new bag(); // 분리시킬 자식 서브트리

    /* 현재 노드에 child[i]의 중간 데이터 삽입 */
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
            /* shallow copy(어떤 노드도 삭제되지 않기 때문에 deep copy를 할
             * 필요가 없다) 후 분리된 자식의 자리는 모두 NULL로 할당 */
            splited_child->child[j] = child[i]->child[MINIMUM + 1 + j];
            child[i]->child[MINIMUM + 1 + j] = NULL;
        }
        splited_child->child_count = MINIMUM + 1;
        child[i]->child_count = MINIMUM + 1;
    }

    insert_child(i + 1, splited_child); // 분리된 자식을 child[i+1]에 삽입
}

template <class Item> bool bag<Item>::erase_one(const Item &target) {
    if (count(target) == 0) // target이 없다면 false 반환
        return false;

    loose_erase(target);
    if ((data_count == 0) && (child_count == 1)) {
        // loose_erase() 후 최상위 노드의 data가 0개인 경우 최상위 노드의 자식을
        // deep copy 후 현재 노드를 clear한 다음에 자식 노드를 현재노드로 deep
        // copy 후 자식 노드를 삭제한다.

        // 자식 노드를 child_copy로 deep copy
        bag *child_copy = bag_copy(child[0]);
        clear(); // 현재 노드 삭제

        // 자식 노드를 현재 노드로 deep copy
        data_count = child_copy->data_count;
        for (size_t i = 0; i < data_count; i++) {
            data[i] = child_copy->data[i];
        }

        child_count = child_copy->child_count;
        for (size_t i = 0; i < child_count; i++) {
            child[i] = bag_copy(child_copy->child[i]);
        }

        // 자식 노드 삭제
        child_copy->clear();
    }

    return true;
}

template <class Item> void bag<Item>::loose_erase(const Item &target) {
    size_t index = 0;

    while (data[index] < target && index < data_count) {
        ++index; // target이 있을 만한 data나 child의 index를 찾는다
    }

    if ((index < data_count) && (data[index] == target)) {
        if (child_count == 0) {
            // 리프노드에서 target을 발견했다면 그냥 삭제한다.
            remove_data(index);
        } else {
            // 리프노드가 아닌 노드에서 target을 찾았다면 왼쪽 서브트리에서 가장
            // 큰 값을 target 자리에 놓고 해당 값을 삭제한다.
            child[index]->remove_biggest(data[index]);

            if (child[index]->data_count == MINIMUM - 1)
                // fix_shortage()는 자식을 fix하는 것이므로 child[i]에 대해서는
                // 여기서 fix해 주어야 한다.
                fix_shortage(index);
        }
    } else if (child_count != 0) {
        // target을 발견하지 못했으면 리프노드에 도달할때까지 재귀적으로 호출
        child[index]->loose_erase(target);

        if (child[index]->data_count == MINIMUM - 1)
            // 재귀적으로 호출후 child[index]가 MINUMUM 조건을 만족하지 않을시
            // fix한다.
            fix_shortage(index);
    }
}

template <class Item> void bag<Item>::fix_shortage(size_t i) {
    assert(i < child_count);                     // Precondition
    assert(child[i]->data_count == MINIMUM - 1); // Precondition

    if (i > 0 && child[i - 1]->data_count > MINIMUM) {
        /* 왼쪽 서브트리에게 데이터를 하나 받아온다 */
        size_t last_index = child[i - 1]->data_count - 1;

        // data[i-1]을 child[i]의 첫번째에 삽입한다
        child[i]->insert_data(0, data[i - 1]);

        // data[i-1]에 child[i-1]의 마지막 데이터를 가져와서 저장한다
        data[i - 1] = child[i - 1]->data[last_index];
        child[i - 1]->remove_data(last_index);

        // child[i]에 자식이 존재한다면 child[i-1]의 마지막 자식을 child[i]의
        // 첫번째 자식으로 삼는다
        if (child[i]->child_count != 0) {
            last_index = child[i - 1]->child_count - 1;
            child[i]->insert_child(0, child[i - 1]->child[last_index]);
            child[i - 1]->remove_child(last_index);
        }
    } else if ((i < child_count - 1) && (child[i + 1]->data_count > MINIMUM)) {
        /* 오른쪽 서브트리에게 데이터를 하나 받아온다 */

        // data[i]를 child[i]의 마지막에 삽입한다
        child[i]->insert_data(MINIMUM - 1, data[i]);

        // child[i+1]의 첫번때 데이터를 data[i]에 저장한다
        data[i] = child[i + 1]->data[0];
        child[i + 1]->remove_data(0);

        // child[i]에 자식이 존재한다면 child[i+1]의 첫번째 자식을 child[i]의
        // 마지막 자식으로 삼는다
        if (child[i]->child_count != 0) {
            child[i]->insert_child(MINIMUM, child[i + 1]->child[0]);
            child[i + 1]->remove_child(0);
        }
    } else if (i > 0) {
        // 왼쪽 child와 merge 한다
        merge_child(i - 1);
    } else {
        // 오른쪽 child와 merge 한다
        merge_child(i);
    }
}

template <class Item> void bag<Item>::remove_biggest(Item &removed_enrty) {
    if (child_count == 0) {
        // 가장 오른쪽 리프노드의 가장 오른쪽 데이터가 가장 큰 값이므로 해당
        // 데이터를 removed_enrty에 저장후 삭제한다
        removed_enrty = data[--data_count];
    } else {
        // 가장 오른쪽 자식노드에 대해 재귀적으로 호출한다
        child[child_count - 1]->remove_biggest(removed_enrty);

        // 가장 오른쪽 자식이 MINIMUM 조건을 불만족하면 fix한다
        if (child[child_count - 1]->data_count == MINIMUM - 1)
            fix_shortage(child_count - 1);
    }
}

template <class Item> void bag<Item>::show_contents() { show_contents_rec(0); }
template <class Item> void bag<Item>::show_contents_rec(size_t depth) {
    int i;
    if (child_count != 0) {
        // 오른쪽 자식을 먼저 재귀적으로 모두 출력후 데이터를 출력한다
        for (i = child_count - 1; i > 0; i--) {
            child[i]->show_contents_rec(depth + 1);
            std::cout << std::setw(4 * depth) << "" << data[i - 1] << std::endl;
        }
        // 가장 왼쪽 자식을 재귀적으로 출력한다
        child[i]->show_contents_rec(depth + 1);
    } else {
        // 리프노드는 자식이 없으므로 데이터를 한번에 모두 출력한다
        for (i = data_count - 1; i >= 0; i--) {
            std::cout << std::setw(4 * depth) << "" << data[i] << std::endl;
        }
    }
}

template <class Item> void bag<Item>::insert_data(size_t i, const Item &entry) {
    assert(i <= data_count); // Precondition

    // 데이터를 한칸씩 뒤로 민다
    for (size_t j = data_count; j > i; j--)
        data[j] = data[j - 1];

    data[i] = entry;
    data_count++;
}

template <class Item> void bag<Item>::insert_child(size_t i, bag *child) {
    assert(i <= child_count); // Precondition

    // 자식을 한칸씩 뒤로 민다
    for (size_t j = child_count; j > i; j--)
        this->child[j] = this->child[j - 1];

    this->child[i] = child;
    child_count++;
}

template <class Item> void bag<Item>::remove_data(size_t i) {
    assert(i < data_count); // Precondition
    data_count--;

    // 데이터를 한칸씩 앞으로 당긴다
    for (; i < data_count; i++)
        data[i] = data[i + 1];
}

template <class Item> void bag<Item>::remove_child(size_t i) {
    assert(i < child_count); // Precondition
    child_count--;

    // 자식을 한칸씩 앞으로 당긴다
    for (; i < child_count; i++)
        child[i] = child[i + 1];
    child[child_count] = NULL;
}

template <class Item> void bag<Item>::merge_child(size_t i) {
    assert(i < child_count - 1); // Precondition
    size_t append_index = child[i]->data_count;
    size_t left_index, right_index;

    // data[i]을 child[i]의 마지막에 삽입한다
    child[i]->insert_data(append_index, data[i]);
    remove_data(i);

    // child[i+1]의 모든 데이터를 child[i]의 끝으로 이동시킨다
    for (left_index = append_index + 1; left_index < MAXIMUM; left_index++) {
        right_index = left_index - append_index - 1;
        child[i]->insert_data(left_index, child[i + 1]->data[right_index]);
    }

    // child[i]에 자식이 존재하면 child[i+1]의 모든 자식을 child[i]의 끝으로
    // 이동시킨다
    if (child[i]->child_count != 0) {
        append_index = child[i]->child_count;
        for (left_index = append_index; left_index <= MAXIMUM; left_index++) {
            right_index = left_index - append_index;
            child[i]->insert_child(left_index,
                                   bag_copy(child[i + 1]->child[right_index]));
        }
    }

    // child[i+1]을 삭제한다
    child[i + 1]->clear();
    remove_child(i + 1);
}

template <class Item> void bag<Item>::clear() {
    if (child_count != 0) {
        // 자식이 존재한다면 리프노드에 도달할때까지 재귀적으로 자식들을 모두
        // delete 한다
        for (size_t i = 0; i < child_count; i++) {
            child[i]->clear(); // 먼저 child[i]의 모든 자식들을 clear() 한다
            delete child[i]; // 그후 child[i]를 delete 한다
            child[i] = NULL; // 또한 child[i]를 NULL로 설정한다
        }
        child_count = 0;
    }
    data_count = 0;
}

template <class Item> bag<Item> *bag_copy(bag<Item> *source) {
    if (source == NULL) // source가 NULL이면 그냥 NULL을 반환한다
        return NULL;

    bag<Item> *copy_node = new bag<Item>(); // 복사해서 반환할 bag

    for (size_t i = 0; i < source->data_count; i++) { // 데이터를 먼저 복사한다
        copy_node->data[i] = source->data[i];
    }
    copy_node->data_count = source->data_count;

    for (size_t i = 0; i < source->child_count; i++) {
        // 자식이 존재할경우 copy_child에 재귀적으로 복사한다
        bag<Item> *copy_child = new bag<Item>();
        copy_child = bag_copy(source->child[i]);

        // 복사한 자식을 cop_node의 자식으로 삼는다
        copy_node->child[i] = copy_child;
    }
    copy_node->child_count = source->child_count;

    return copy_node;
}
