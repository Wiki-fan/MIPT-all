#include <iostream>
#include "FSM.h"

int main() {
    FSM<> fsm;
    //std::string s = "(a+b).c";
    //std::string s = "(a+b)*.a.b.b.(a+b)*";
    //std::string s = "b*.(a.a)*.(b.b*.(a.a)*)*.b.b*";
    //std::string s = "b*.(a+a.b)*.(b.b*.(a.a)*)*.b.b*";
    std::string s = "(a.(a+a.b).b.(a.b+b.a))*";
    std::string rpn =fsm.fromInfixToRpn(s);
    std::cout << rpn <<std::endl;
    fsm.fromReversePolishToFsm(rpn);
    std::cout <<"FSM" <<std::endl <<fsm.serialize();

    fsm.removeEpsilonTransitions();
    std::cout <<"Without Epsilon transitions" <<std::endl << fsm.serialize();
    std::cout <<fsm.serializeForGraphEditor();
    fsm = fsm.determinize();
    std::cout <<"Determinized" <<std::endl << fsm.serialize();
    fsm = fsm.complete();
    std::cout <<"Completed" <<std::endl << fsm.serialize();
    fsm = fsm.minimize();
    std::cout <<"Minimized" <<std::endl << fsm.serialize();

    return 0;
}
