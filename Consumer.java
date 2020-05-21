/*
 * Consumer.java
 *
 * Created on 21 January 2008, 22:55
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */

package producerconsumer;

/**
 *
 * @author CHRIS
 */
public class Consumer extends Thread{
    Buffer buf;
    int id;
    public Consumer(Buffer b, int consumerNum) {
        buf = b;
        id = consumerNum;
    }
    public boolean consume(){
        String nextItem = buf.extract();
        System.out.println("Consumer "+id+" consuming: "+nextItem);
        return !nextItem.equals("****");
    }

    public void run(){
        while(consume()){
            try {
                sleep( (int) (Math.random() * 5));
            } catch (InterruptedException ex) {
            }
        }
        buf.insert("****");
    }
}
