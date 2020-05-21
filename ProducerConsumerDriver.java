package producerconsumer;

/**
 * <p>Title: Producer-Consumer</p>
 *
 * <p>Description: Producer-Consumer Problem with synchronized</p>
 *
 * <p>Copyright: Copyright (c) 2006</p>

 *
 * @author Chris Casey
 * @version 1.0
 */


public class ProducerConsumerDriver {
    public ProducerConsumerDriver() {
    }

    public static void main(String[] args) {
       Buffer b = new Buffer(2);
       Consumer c = new Consumer(b,1);
       Consumer c2 = new Consumer(b,2);
       Producer p = new Producer(1, b, 5);
       Producer p2 = new Producer(2, b, 5);
       c.start();
       c2.start();
       p.start();
       p2.start();
       try {
           p.join();
           p2.join();
       } catch (InterruptedException ex) {
       }
       b.insert("****");
        try {
           c.join();
           c2.join();
       } catch (InterruptedException ex) {
       }
    }
}
