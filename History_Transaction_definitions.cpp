#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 1
//
Transaction::Transaction (std::string ticker_symbol, unsigned int day_date, unsigned int month_date, unsigned year_date, 
bool buy_sell_trans, unsigned int number_shares, double trans_amount) {
  
  trans_id = assigned_trans_id;
  symbol = ticker_symbol;
  day = day_date; 
  month = month_date;
  year = year_date; 
  shares = number_shares; 
  amount = trans_amount; 

  if ( buy_sell_trans ) {
    trans_type = "Buy";
  }else {
    trans_type = "Sell"; 
  }

  assigned_trans_id ++;
  p_next=nullptr;

}


// Destructor
// TASK 1
//
Transaction::~Transaction() {
  p_next = nullptr;
}

// Overloaded < operator.
// TASK 2
//
bool Transaction:: operator < ( Transaction const &other ) {
  if (year <  other.year ) {
    return true; 
  }
  
  if (year == other.year && month < other.month) {
    return true;
  }

  if (year == other.year && month == other.month && day < other.day){
    return true; 
  }

  if (year == other.year && month == other.month && day == other.day && trans_id > other.trans_id){
    return true; 
  }

  return false;
}

// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK 3
//
History::History (){
  p_head = 0; 
}

// Destructor
// TASK 3
//
History::~History(){
  Transaction *current_trans = p_head; 

  // if (p_head == nullptr) {
  //   delete p_head; 
  // }

  while (p_head != nullptr){
    p_head = p_head -> get_next(); 
    delete current_trans; 
    current_trans = nullptr; 
    current_trans = p_head;
  } 

}

// read_history(...): Read the transaction history from file.
// TASK 4
//
void History::read_history (){

  ece150::open_file (); 

  while (ece150::next_trans_entry()) {

    Transaction *new_trans { new Transaction (ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), 
    ece150::get_trans_year(), ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount())};

    insert (new_trans);
  }

  ece150::close_file (); 

}

// insert(...): Insert transaction into linked list.
// TASK 5
//
void History::insert ( Transaction *p_new_trans ){
  Transaction *current_trans = p_head; 
  
  if (p_head == nullptr) {
    p_head = p_new_trans; 
  } 
  else{
    current_trans=p_head;
  while (current_trans -> get_next() != nullptr) {
    current_trans = current_trans -> get_next(); 
  }

  current_trans -> set_next (p_new_trans);
  }
}

// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//
void History::sort_by_date(){

  Transaction *previous_trans = NULL; 
  Transaction *next_trans = NULL; 
  Transaction *current_trans = p_head; 
  int swapped = 0; 

  while (true) {

    //swaps one round
    while ( (current_trans -> get_next()) != NULL ){

      //swaps the two trans 
      if ( *(current_trans -> get_next()) < *current_trans ){
        
        swapped ++; 

        //if it starts at p_head again
        if ( previous_trans == NULL ){
          //current is first trans 
          //next is second trans 
          next_trans = current_trans -> get_next();
          //second trans is now the third trans 
          current_trans -> set_next (next_trans -> get_next()); 
          //second trans is the first one (head)
          next_trans -> set_next (current_trans); 
          previous_trans = next_trans; 
          p_head = next_trans; 
        }

        else {
          //previous -> current -> next
          next_trans = current_trans -> get_next();
          //sets the third trans to the second position 
          //previous -> next -> current 
          previous_trans -> set_next (next_trans); 
          //previous -> next -> current -> next trans 
          current_trans -> set_next (next_trans -> get_next());
          //previous -> next -> current -> next trans 
          next_trans -> set_next (current_trans); 
          previous_trans = next_trans; 
        }

      }

      else {
        previous_trans = current_trans; 
        current_trans = current_trans -> get_next(); 
      }
      
    }

    // the list has not swapped once this round 
    if ( swapped == 0 ) {
      break; 
    } 
    
    // the list has swapped at least once this round 
    else if ( swapped >= 1 ) {
      swapped = 0; 
      previous_trans = NULL; 
      next_trans = NULL;
      current_trans = p_head;
    } 

  } 

}

// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//
void History::update_acb_cgl(){

 Transaction *current_trans = p_head; 
  
  double acb = current_trans -> get_acb(); //current_trans -> get_amount(); 
  int share_balance = current_trans -> get_share_balance(); 
  double acb_per_share = acb/share_balance;
  double cgl = current_trans -> get_cgl(); 

  current_trans -> set_acb ( acb );
  current_trans -> set_share_balance ( share_balance );
  current_trans -> set_acb_per_share ( acb_per_share ); 
  current_trans -> set_cgl ( cgl );

  while ( (current_trans -> get_next()) != nullptr ){
    current_trans = current_trans -> get_next();
    
    //trade is buy 
    if ( current_trans -> get_trans_type() ) {
      acb = acb + ( current_trans -> get_amount()); //current_trans  -> get_amount() );
      share_balance = share_balance + ( current_trans  -> get_shares() ); 
      acb_per_share = acb/share_balance; //( current_trans -> get_acb_per_share() ); 
    } 
    
    //trade is sell
    else {
      acb = acb - ( (current_trans -> get_shares()) * acb_per_share ); 
      share_balance = share_balance - (current_trans -> get_shares() );
      cgl =  (current_trans -> get_amount() ) - ( (current_trans -> get_shares()) * acb_per_share );
    }

    current_trans -> set_acb ( acb );
    current_trans -> set_share_balance ( share_balance );
    current_trans -> set_acb_per_share ( acb_per_share ); 
    current_trans -> set_cgl ( cgl ); 
     
  } 

}


// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8
double History::compute_cgl ( unsigned int year ){
  Transaction *current_trans = p_head; 
  double total_cgl = 0; 

  if ( (current_trans -> get_year()) == year ){
    total_cgl = total_cgl + ( current_trans -> get_cgl() );
  }

  while ( current_trans -> get_next() != nullptr ){
    current_trans = current_trans -> get_next(); 

    if ( (current_trans -> get_year()) == year){
      total_cgl = total_cgl + ( current_trans -> get_cgl() );
    }

  }

  return total_cgl; 
}



// print() Print the transaction history.
//TASK 9
//
void History::print(){
  Transaction *current_trans = p_head; 
  
  std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;

  while (current_trans != nullptr) {
    current_trans -> print(); 
    current_trans = current_trans -> get_next(); 
  }

  std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;

}


// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
