#include "../include/simulator.h"
#include "../include/helper.h"
#include<iostream>
#include<string>
#include<vector>
using namespace std;

#define BUFFER 1000
#define MSG_SIZE 20
#define INTERRUPT 12.0

string buffer[BUFFER];
struct pkt buf[BUFFER];
int seqno[BUFFER];
int index = 0;
int acked = 0;
int acks[BUFFER];
int win_start = 0;
int win_end = 0;
int window_size;

struct pkt gen_pkt(string message, int seqnum) {
    struct pkt p;
    p.seqnum = seqnum;
    p.acknum = seqnum;
    for(int i = 0; i < MSG_SIZE; i++) {
        p.payload[i] = (char)message[i];
    }
    p.checksum = 0;
    for(int i = 0; i < MSG_SIZE; i++) {
        p.checksum += (int)p.payload[i];
    }
    p.checksum += p.seqnum + p.acknum;
    return p;
}

bool validate_checksum(struct pkt p) {
    int checksum = 0;
    for(int i = 0; i < MSG_SIZE; i++) {
        checksum += (int)p.payload[i];
    }
    checksum += p.seqnum + p.acknum;
    if(checksum == p.checksum) {
        return true;
    } else {
        return false;
    }
}

/* ******************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for PA2, unidirectional data transfer
   protocols (from A to B). Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
     (although some can be lost).
**********************************************************************/

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

/* called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{

    buf[index] = gen_pkt((char*)message.data,index);
    buffer[index] = (char*)message.data;
    if(index < win_end) {
        if(index == win_start) {
            starttimer(0,INTERRUPT);
        }
        tolayer3(0,buf[index]);
        acks[index] = 0;
    }
    index++;
    cout<<index<<endl;
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{

    if(validate_checksum(packet)) {
        if(acks[packet.acknum] != 0) {
            return;
        }
        win_start = packet.acknum + 1;
        win_end = win_start + window_size;
        acks[packet.acknum] = 1;
        buffer[packet.acknum] = "";
        stoptimer(0);
        if(win_start != index) {
            starttimer(0,INTERRUPT);
        }
    }
}

/* called when A's timer goes off */
void A_timerinterrupt()
{
    for(int i = win_start; i < win_end && i < BUFFER; i++) {
        if(buffer[i] == "") {
            return;
        } else if(acks[i] == 0){
            if(i == win_start) {
                starttimer(0,INTERRUPT);
            }
            tolayer3(0,buf[i]);
        } else {
            break;
        }

    }

}

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
    for(int i = 0; i < BUFFER; i++) {
        seqno[i] = i;
        acks[i] = -1;
    }
    win_end = getwinsize();
    window_size = getwinsize();
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */
int b_ack = 0;
struct pkt latest;
/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet)
{
    if(validate_checksum(packet)) {

        if(b_ack == packet.acknum) {
            tolayer5(1,packet.payload);
            b_ack+=1;
            latest = packet;

        }

        tolayer3(1,latest);
    }
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{

}
