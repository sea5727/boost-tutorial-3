#include <iostream>
#include <vector>
class Object{
    int a;
};


void sample1()
{
    auto a = Object{};
    auto b = a; //복사 생성
}
void sample2(){
    auto a = Object{};
    auto b = Object{};
    std::swap(a, b);// 자원 교환
}
void sample3(){
    auto a = Object{};
    auto b = std::move(a); // b로 자원을 이동
}
void sample4(){
    const auto a0 = std::vector<int> {1, 2, 3, 4, 5, 6};
    const auto a1 = std::vector<int>{7, 8, 9};

    auto b = a0; // 복사생성
    b = a1; // 복사 할당
}

class Buffer{
private:
    size_t size_{ 0 };
    float * ptr_{ nullptr };
public:
    //생성자
    Buffer(const std::initializer_list<float> & values) 
        :   size_(values.size())
    {
        ptr_ = new float[values.size()];
        std::copy(values.begin(), values.end(), ptr_);
    }
    //복사 생성자
    Buffer(const Buffer & other) 
        :    size_(other.size_)
    {
        ptr_ = new float[size_];
        std::copy(other.ptr_, other.ptr_ + size_, ptr_);
    }  
    //복사 할당
    auto & operator=(const Buffer & other){
        delete [] ptr_;
        ptr_ = new float[other.size_];
        size_ = other.size_;
        std::copy(other.ptr_, other.ptr_ + size_, ptr_);
        return *this;
    }
    // 이동 생성자
    Buffer(Buffer && other) noexcept 
        :   ptr_{other.ptr_}
        ,   size_{other.size_}
    {
        other.ptr_ = nullptr;
        other.size_ = 0;
    }
    auto & operator=(Buffer && other) noexcept{
        ptr_ = other.ptr_;
        size_ = other.size_;
        other.ptr_ = nullptr;
        other.size_ = 0;
        return *this;
    }
    // 소멸자
    ~Buffer(){
        delete[] ptr_;
        ptr_ = nullptr;
    }

    auto begin() const { return ptr_; }
    auto end() const { return ptr_ + size_; }
};

class Bird{
public:
    Bird() {}
    //set_song  어느것이 사용될까?
    auto set_song(const std::string & s) { song_ = s;}
    auto set_song(std::string && s) { song_ = std::move(s); }
    std::string song_;
};
auto make_roast_song() { return std::string{"I'm a Roast"};}
void sample5(){
    auto bird = Bird();
    auto cuckoo_a = std::string{ "I'm a Cuckoo"}; 
    bird.set_song(cuckoo_a); // 복사 할당


    auto cuckoo_b = std::string{ "I'm a Cuckoo"}; 
    bird.set_song(std::move(cuckoo_b)); /// 이동 할당

    bird.set_song(make_roast_song()); // 이동 할당

    auto roast_song_a = make_roast_song();
    bird.set_song(roast_song_a); // 복사 할당

    const auto roast_song_a = make_roast_song();
    bird.set_song(std::move(roast_song_a)); // 복사 할당 



}
int main(int argc, char** argv) {
    auto a = Object{};
    auto b = Object{};

    std::cout << "swap tutorial" << std::endl;
    return 0;

}

