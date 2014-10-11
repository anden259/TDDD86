/*
 * anden259
 * andno037
 *
 */
#include "anden259_andno037.h"
#include "view.h"


anden259_andno037::anden259_andno037():
    Tank {} {
    this->name = "anden259_andno037";
}

void anden259_andno037::update_view(const sensors &s)
{




}

void anden259_andno037::update_status(const sensors &s)
{


    board.set_status(s.me.r-1,s.me.c-1,s.look[0]);

}



action anden259_andno037::doYourThing(const sensors &s)
{

    return goToOppBase(s);

    // right now calls base class fireAtOpp method -
    // feel free to override that method
    // with one that fires at where you think they will be
}

string anden259_andno037::taunt(const string &otherguy) const
{
    return "You couldn't code your way out of a wet paper bag, " + otherguy;
}

