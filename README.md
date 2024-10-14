# std::inplace_string
The interface is identical to std::string, except for the following differences
* `get_allocator` has been removed
* All overloads of functions taking allocators have been removed.
* `try_push_back` and `unchecked_push_back` alongside `push_back`.
* `try_append_range` alongside `append_range`

This matches the delta between `std::vector` and `std::inplace_vector`.

Progress:
```
WIP     (constructor)
DONE    (destructor)
        operator=
        assign
        assign_range
        
DONE    at
DONE    operator[]
DONE    front
DONE    back
DONE    data
DONE    c_str
DONE    operator basic_string_view
        
DONE    begin
DONE    cbegin
DONE    end
DONE    cend
DONE    rbegin
DONE    crbegin
DONE    rend
DONE    crend
        
DONE    empty
DONE    size
DONE    length
DONE    max_size
DONE    reserve
DONE    capacity
DONE    shrink_to_fit
        
DONE    clear
        insert
        insert_range
        erase
DONE    push_back
DONE    try_push_back
DONE    unchecked_push_back
        pop_back
        append
        append_range
        try_append_range
        operator+=
        replace
        replace_with_range
        copy
        resize
        resize_and_override
        swap
        
DONE    find
DONE    rfind
DONE    find_first_of
DONE    find_first_not_of
DONE    find_last_of
DONE    find_last_not_of
        
        copmare
        starts_with
        ends_width
        contains
        substr
        
        npos
```