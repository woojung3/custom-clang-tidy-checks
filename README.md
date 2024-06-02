# custom-clang-tidy-checks
custom clang-tidy checks (mostly aliases)

## Install
llvm clone:
```bash
git clone https://github.com/llvm/llvm-project.git
cd llvm-project
mkdir build
cmake -G Ninja -S ./llvm -B ./build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra"
```

add autocrypt checker to clang-tidy

clang-tools-extra/clang-tidy/CMakeLists.txt:
- add `add_subdirectory(autocrypt)`
- add `set(ALL_CLANG_TIDY_CHECKS ... clangTidyAutocryptModule ...)`

clang-tools-extra/clang-tidy/ClangTidyForceLinker.h:
```cpp
// This anchor is used to force the linker to link the AutocryptModule.
extern volatile int AutocryptModuleAnchorSource;
static int LLVM_ATTRIBUTE_UNUSED AutocryptModuleAnchorDestination =
	AutocryptModuleAnchorSource;
```

## Build
```bash
ninja -C build clang-tidy
```

## Checks
```bash
❯ ~/clang-tidy --checks='-*,autocrypt*' --list-checks
Enabled checks:
    autocrypt-mobis-cert-cpp-con54
    autocrypt-mobis-cert-cpp-ctr54
    autocrypt-mobis-cert-cpp-ctr55
    autocrypt-mobis-cert-cpp-ctr56
    autocrypt-mobis-cert-cpp-dcl50
    autocrypt-mobis-cert-cpp-dcl51
    autocrypt-mobis-cert-cpp-dcl54
    autocrypt-mobis-cert-cpp-dcl57
    autocrypt-mobis-cert-cpp-dcl58
    autocrypt-mobis-cert-cpp-dcl59
    autocrypt-mobis-cert-cpp-err52
    autocrypt-mobis-cert-cpp-err58
    autocrypt-mobis-cert-cpp-err61
    autocrypt-mobis-cert-cpp-err62
    autocrypt-mobis-cert-cpp-exp54
    autocrypt-mobis-cert-cpp-exp55
    autocrypt-mobis-cert-cpp-exp63
    autocrypt-mobis-cert-cpp-msc50
    autocrypt-mobis-cert-cpp-msc51
    autocrypt-mobis-cert-cpp-oop54
    autocrypt-mobis-cert-cpp-oop57
    autocrypt-mobis-cert-cpp-oop58
```

## Test
> autocrypt-chekcer-test/CMakeLists.txt: in `find_program(CLANG_TIDY ...`
> adjust clang-tidy path with HINTS argument

```bash
cd autocrypt-checker-test
mkdir build
cd build
cmake ..
make -j
```

---

## Checks (in detail)

| Rule  | 개요                                                                                                            | Clang 매핑                                                                             | 비고                                                                                                |
| ----- | ------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------- |
| DCL50 | Do not define a C-style variadic function                                                                     | cert-dcl50-cpp                                                                       |                                                                                                   |
| DCL51 | Do not declare or define a reserved identifier                                                                | cert-dcl51-cpp(= bugprone-reserved-identifier)                                       |                                                                                                   |
| DCL52 | Never qualify a reference type with const or volatile                                                         | clang-diagnostic-error                                                               | clang-diagnostic-error 항목은 체커 등록 없이도 항상 체크됨                                                       |
| DCL53 | Do not write syntactically ambiguous declarations                                                             | x                                                                                    | clang 컴파일러 사용시 -Wvexing-parse 옵션 적용으로 검지 가능. 객체 생성시 ()대신 {} 사용으로 대부분 막을 수 있음                      |
| DCL54 | Overload allocation and deallocation functions as a pair in the same scope                                    | misc-new-delete-overloads                                                            |                                                                                                   |
| DCL55 | Avoid information leakage when passing a class object across a trust boundary                                 | x                                                                                    | 작성할 일이 많지 않은 코드임. 지속적으로 검사할 필요 없음                                                                 |
| DCL56 | Avoid cycles during initialization of static objects                                                          | x                                                                                    | 작성할 일이 많지 않은 코드임. 지속적으로 검사할 필요 없음                                                                 |
| DCL57 | Do not let exceptions escape from destructors or deallocation functions                                       | bugprone-exception-escape                                                            |                                                                                                   |
| DCL58 | Do not modify the standard namespaces                                                                         | cert-dcl58-cpp                                                                       |                                                                                                   |
| DCL59 | Do not define an unnamed namespace in a header file                                                           | cert-dcl59-cpp                                                                       |                                                                                                   |
| DCL60 | Obey the one-definition rule                                                                                  | x                                                                                    | 작성할 일이 많지 않은 코드임. 지속적으로 검사할 필요 없음(헤더 온리 라이브러리, 템플릿 코드를 여러 버전에 걸쳐 혼용하는 등으로 발생 가능)                  |
| EXP50 | Do not depend on the order of evaluation for side effects                                                     | x                                                                                    | Clang: -Wunsequenced / GCC: -Wsequence-point 로 검지 가능                                              |
| EXP51 | Do not delete an array through a pointer of the incorrect type                                                | clang-analyzer-cplusplus.ArrayDelete                                                 | clang-analyzer 계열은 clang-tidy가 아닌 clang static analyzer 기능임. 이 계열 check에 대해서는 check aliases 작성 불가 |
| EXP52 | Do not rely on side effects in unevaluated operands                                                           | x                                                                                    | 작성할 일이 많지 않은 코드임. 지속적으로 검사할 필요 없음(Clang: -Wunevaluated-expression)                                |
| EXP53 | Do not read uninitialized memory                                                                              | clang-analyzer-core.UndefinedBinaryOperatorResult 로 일부 검지 가능                         | Clang: -Wuninitialized / GCC: -Wuninitialized 로 검지 가능                                             |
| EXP54 | Do not access an object outside of its lifetime                                                               | bugprone-dangling-handle                                                             | Clang: -Wdangling-initializer-list                                                                |
| EXP55 | Do not access a cv-qualified object through a cv-unqualified type                                             | cppcoreguidelines-pro-type-const-cast                                                |                                                                                                   |
| EXP58 | Pass an object of the correct type to va_start                                                                | x                                                                                    | DCL50에 의해 va_start는 사용금지됨(Clang: -Wvarargs)                                                       |
| EXP59 | Use offsetof() on valid types and members                                                                     | x                                                                                    | Clang: -Winvalid-offsetof / GCC: -Winvalid-offsetof                                               |
| EXP60 | Do not pass a nonstandard-layout type object across execution boundaries                                      | x                                                                                    | 공개 API에 노출된 구조체에 static_assert 걸어 해결(Clang: -Wdynamic-class-memaccess)                            |
| EXP61 | A lambda object must not outlive any of its reference captured objects                                        | x                                                                                    | 람다를 포함한 PR은 capture 내용 주의깊게 검토할 것                                                                 |
| EXP62 | Do not access the bits of an object representation that are not part of the object's value representation     | x                                                                                    | 작성할 일이 많지 않은 코드임. 지속적으로 검사할 필요 없음                                                                 |
| EXP63 | Do not rely on the value of a moved-from object                                                               | bugprone-use-after-move                                                              |                                                                                                   |
| INT50 | Do not cast to an out-of-range enumeration value                                                              | clang-analyzer-optin.core.EnumCastOutOfRange                                         |                                                                                                   |
| CTR50 | Guarantee that container indices and iterators are within the valid range                                     | x                                                                                    | 실수하기 쉬움. 주의해야 함                                                                                   |
| CTR51 | Use valid references, pointers, and iterators to reference elements of a container                            | x                                                                                    | 실수하기 쉬움. 주의해야 함                                                                                   |
| CTR52 | Guarantee that library functions do not overflow                                                              | x                                                                                    | 실수하기 쉬움. 주의해야 함                                                                                   |
| CTR53 | Use valid iterator ranges                                                                                     | x                                                                                    | 실수하기 쉬움. 주의해야 함                                                                                   |
| CTR54 | Do not subtract iterators that do not refer to the same container                                             | cppcoreguidelines-pro-bounds-pointer-arithmetic                                      | 포인터, 이터레이터간 연산 지양                                                                                 |
| CTR55 | Do not use an additive operator on an iterator if the result would overflow                                   | cppcoreguidelines-pro-bounds-pointer-arithmetic                                      | 포인터, 이터레이터간 연산 지양                                                                                 |
| CTR56 | Do not use pointer arithmetic on polymorphic objects                                                          | cppcoreguidelines-pro-bounds-pointer-arithmetic                                      | 포인터, 이터레이터간 연산 지양                                                                                 |
| STR50 | Guarantee that storage for strings has sufficient space for character data and the null terminator            | x                                                                                    | char \* 대신 std::string 사용하면 됨. char\*나 char\[\] 사용을 지양시킬 것                                        |
| STR51 | Do not attempt to create a std::string from a null pointer                                                    | clang-analyzer-cplusplus.StringChecker                                               |                                                                                                   |
| STR52 | Use valid references, pointers, and iterators to reference elements of a basic_string                         | x                                                                                    | 작성할 일이 많지 않은 코드임. 지속적으로 검사할 필요 없음(string 가공을 할 일이 많지 않음)                                          |
| STR53 | Range check element access                                                                                    | x                                                                                    | 작성할 일이 많지 않은 코드임. 지속적으로 검사할 필요 없음                                                                 |
| MEM50 | Do not access freed memory                                                                                    | clang-analyzer-cplusplus.NewDelete /<br>clang-analyzer-alpha.security.ArrayBoundV2   |                                                                                                   |
| MEM51 | Properly deallocate dynamically allocated resources                                                           | clang-analyzer-cplusplus.NewDeleteLeaks<br>clang-analyzer-unix.MismatchedDeallocator |                                                                                                   |
| MEM52 | Detect and handle memory allocation errors                                                                    | x                                                                                    | 가능한한 RAII 래퍼를 작성하시오                                                                               |
| MEM53 | Explicitly construct and destruct objects when manually managing object lifetime                              | x                                                                                    | 가능한한 RAII 래퍼를 작성하시오                                                                               |
| MEM54 | Provide placement new with properly aligned pointers to sufficient storage capacity                           | x                                                                                    | alignas 혹은 std::aligned_storage 사용                                                                |
| MEM56 | Do not store an already-owned pointer value in an unrelated smart pointer                                     | x                                                                                    | RAII pointer에서 raw pointer 전환시 주의깊게 리뷰할 것                                                         |
| FIO50 | Do not alternately input and output from a file stream without an intervening positioning call                | x                                                                                    | FIO 래퍼 작성하여 방어적으로 사용할 것                                                                           |
| FIO51 | Close files when they are no longer needed                                                                    | x                                                                                    | fstream RAII 객체 이용                                                                                |
| ERR50 | Do not abruptly terminate the program                                                                         | x                                                                                    | 작성할 일이 많지 않은 코드임. 지속적으로 검사할 필요 없음                                                                 |
| ERR51 | Handle all exceptions                                                                                         | x                                                                                    | throw 조심해서 사용                                                                                     |
| ERR52 | Do not use setjmp() or longjmp()                                                                              | cert-err52-cpp                                                                       |                                                                                                   |
| ERR53 | Do not reference base classes or class data members in a constructor or destructor function-try-block handler | x                                                                                    | 작성할 일이 많지 않은 코드임. 지속적으로 검사할 필요 없음                                                                 |
| ERR54 | Catch handlers should order their parameter types from most derived to least derived                          | x                                                                                    | 작성할 일이 많지 않은 코드임. 지속적으로 검사할 필요 없음(Clang: -Wexceptions)                                            |
| ERR55 | Honor exception specifications                                                                                | x                                                                                    | 작성할 일이 많지 않은 코드임. 지속적으로 검사할 필요 없음(noexcept는 정말 사실인 경우에만 사용)                                       |
| ERR56 | Guarantee exception safety                                                                                    | x                                                                                    | 작성할 일이 많지 않은 코드임. 지속적으로 검사할 필요 없음                                                                 |
| ERR57 | Do not leak resources when handling exceptions                                                                | x                                                                                    | RAII 준수                                                                                           |
| ERR58 | Handle all exceptions thrown before main() begins executing                                                   | cert-err58-cpp                                                                       |                                                                                                   |
| ERR61 | Catch exceptions by lvalue reference                                                                          | cert-err61-cpp                                                                       |                                                                                                   |
| ERR62 | Detect errors when converting a string to a number                                                            | cert-err34-c                                                                         |                                                                                                   |
| OOP50 | Do not invoke virtual functions from constructors or destructors                                              | clang-analyzer-alpha.cplusplus.VirtualCall                                           |                                                                                                   |
| OOP51 | Do not slice derived objects                                                                                  | x                                                                                    | C++에서 다형성은 포인터나 레퍼런스 사용시 얻을 수 있음. 부모 객체에 value copy 하지 마시오                                        |
| OOP52 | Do not delete a polymorphic object without a virtual destructor                                               | x                                                                                    | 상속 지양                                                                                             |
| OOP53 | Write constructor member initializers in the canonical order                                                  | x                                                                                    | 클래스 멤버 변수의 선언 순서대로 생성자 멤버 초기화가 수행됨. 순서를 맞추시오                                                      |
| OOP54 | Gracefully handle self-copy assignment                                                                        | cert-oop54-cpp                                                                       |                                                                                                   |
| OOP55 | Do not use pointer-to-member operators to access nonexistent members                                          | x                                                                                    | 작성할 일이 많지 않은 코드임. 지속적으로 검사할 필요 없음                                                                 |
| OOP57 | Prefer special member functions and overloaded operators to C Standard Library functions                      | cert-oop57-cpp                                                                       |                                                                                                   |
| OOP58 | Copy operations must not mutate the source object                                                             | cert-oop58-cpp                                                                       |                                                                                                   |
| CON50 | Do not destroy a mutex while it is locked                                                                     | x                                                                                    | 병렬화 코드 작성/리뷰시 주의                                                                                  |
| CON51 | Ensure actively held locks are released on exceptional conditions                                             | x                                                                                    | 병렬화 코드 작성/리뷰시 주의                                                                                  |
| CON52 | Prevent data races when accessing bit-fields from multiple threads                                            | x                                                                                    | 병렬화 코드 작성/리뷰시 주의                                                                                  |
| CON53 | Avoid deadlock by locking in a predefined order                                                               | x                                                                                    | 병렬화 코드 작성/리뷰시 주의                                                                                  |
| CON54 | Wrap functions that can spuriously wake up in a loop                                                          | cert-con54-cpp                                                                       | 병렬화 코드 작성/리뷰시 주의                                                                                  |
| CON55 | Preserve thread safety and liveness when using condition variables                                            | x                                                                                    | 병렬화 코드 작성/리뷰시 주의                                                                                  |
| CON56 | Do not speculatively lock a non-recursive mutex that is already owned by the calling thread                   | x                                                                                    | 병렬화 코드 작성/리뷰시 주의                                                                                  |
| MSC50 | Do not use std::rand() for generating pseudorandom numbers                                                    | cert-msc50-cpp                                                                       |                                                                                                   |
| MSC51 | Ensure your random number generator is properly seeded                                                        | cert-msc51-cpp                                                                       |                                                                                                   |
| MSC52 | Value-returning functions must return a value from all exit paths                                             | x                                                                                    | 분기가 많은 함수 작성시 반환값 누락 없도록 주의                                                                       |
| MSC53 | Do not return from a function declared \[\[noreturn\]\]                                                       | x                                                                                    | noreturn 기재시 반환값 없도록 주의                                                                           |

