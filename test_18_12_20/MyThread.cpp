//
// Created by Петр on 18.12.2020.
//

class MyRAIIThread{
public:
    MyRAIIThread(std::thread& t) : th(t) {
    }

    ~MyRAIIThread(){
        if(th.joinable()){
            th.join();
            std::cout << "Joined" << std::endl;
        }
    }
    MyRAIIThread(const MyRAIIThread &)= delete;

    MyRAIIThread& operator=(const MyRAIIThread &)= delete ;

private:
std::thread& th;
};

