/*
 * Producer.java
 *
 * Created on 21 January 2008, 22:53
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */

package producerconsumer;

/**
 *
 * @author CHRIS
 */
public class Producer extends Thread  {
    Buffer buf;
    int id;
    int number;
    public Producer(int id, Buffer b, int numItems){
        this.id = id;
        buf = b;
        number = numItems;
    }
    public void produce(int i){
        System.out.println("Producer "+id+" producing item: "+i);
        buf.insert("item: "+i+"(Producer "+id+")");
    }

    public void run() {
        for (int i=0; i < number; i++){
            try {
                sleep((int) (Math.random() * 5));
            } catch (InterruptedException ex) {
            }
             produce(i);
        }
       
    }
}