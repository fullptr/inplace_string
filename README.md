# std::inplace_string
The interface is identical to std::string, except for the following differences
* `get_allocator` has been removed
* `allocator_type` member type has been removed
* No pmr typedefs
* All overloads of functions taking allocators have been removed.
* `try_push_back` and `unchecked_push_back` alongside `push_back`
* `try_append_range` alongside `append_range`

This matches the delta between `std::vector` and `std::inplace_vector`.

Progress:
```
DONE    # Member Types
DONE    traits_type
DONE    value_type
DONE    size_type
DONE    difference_type
DONE    reference
DONE    const_reference
DONE    pointer
DONE    const_pointer
DONE    iterator
DONE    const_iterator
DONE    reverse_iterator
DONE    const_reverse_iterator

        # Member Functions
WIP     (constructor)
DONE    (destructor)
        operator=
        assign
        assign_range
        
DONE    # Element Access
DONE    at
DONE    operator[]
DONE    front
DONE    back
DONE    data
DONE    c_str
DONE    operator basic_string_view
        
DONE    # Iterators
DONE    begin
DONE    cbegin
DONE    end
DONE    cend
DONE    rbegin
DONE    crbegin
DONE    rend
DONE    crend
        
DONE    # Capacity
DONE    empty
DONE    size
DONE    length
DONE    max_size
DONE    reserve
DONE    capacity
DONE    shrink_to_fit
        
        # Modifiers
DONE    clear
        insert
        insert_range
        erase
DONE    push_back
DONE    try_push_back
DONE    unchecked_push_back
DONE    pop_back
        append
        try_append
        append_range
        try_append_range
        operator+=
        replace
        replace_with_range
        copy
        resize
        resize_and_override
        swap
        
        # Search
DONE    find
DONE    rfind
DONE    find_first_of
DONE    find_first_not_of
DONE    find_last_of
DONE    find_last_not_of
        
        # Operations
        compare
        starts_with
        ends_width
        contains
        substr
        
        npos

        # Non-member functions
        operator+
        operator==
        operator<=>
        std::swap
        erase
        erase_if

        # Input/Output
        operator<<
        operator>>
        getline

        # Numeric conversions
        stoi
        stol
        stoll
        stoul
        stoull
        stof
        stod
        stold
        to_string
        to_wstring

        # Literals
        operator""is

        # Helper Classes
        std::hash<fp::inplace_string
        
        # Deduction Guides
```
## Open Questions
* Should iterators for different size inplace_strings be different types. Surely yes, but the current implementation doesn't.
* How should size_type, difference_type, pointer and const_pointer be specified? string gets them from the allocator_traits, whereas string_view just uses the obvious size_t, ptrdiff_t, CharT* and const CharT* respectively. inplace_vector also does what string_view does (but with value_type instead of CharT for the pointer typedefs), so that's what I've gone with here.