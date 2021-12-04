import java.io.*;
import java.net.*;
import java.util.Scanner;

public class GameClient {
    public static void main(String[] args) throws IOException {
        InetAddress addr = InetAddress.getByName("localhost"); // IP アドレスへの変換
        System.out.println("接続中...\n");
        Socket socket = new Socket(addr, GameServer.PORT); // ソケットの生成
        try {
            System.out.println("対戦相手がみつかりました\n");
            BufferedReader in = new BufferedReader( new InputStreamReader(socket.getInputStream())); // データ受信用バッファの設定
            PrintWriter out = new PrintWriter( new BufferedWriter( new OutputStreamWriter(socket.getOutputStream())), true); // 送信バッファ設定
            Scanner scan = new Scanner(System.in);
            
            Game newgame = new Game();
            newgame.Rule();
            int check = 0;
            while(check!=1){
                check = scan.nextInt();
            }
            out.println("ok");
            while(!in.readLine().equals("ok"));

            newgame.Gamestart_Client(in, out);
        

            //Gamestart_Serverから Cliantの役割をうけとる
            String c_role; 
            c_role = in.readLine();
            if(c_role.equals("DangerousNum")){
                System.out.println("対戦相手が「一度に進められる数字の数」を決定しています......\n");
                int c_num = Integer.parseInt(in.readLine());
                newgame.C_Num = c_num;
                System.out.printf("----------一度に進められる数字の数は%dになりました----------\n\n", c_num);
                newgame.DecideRules_sub(c_role, in, out);
            }else{
                newgame.DecideRules_sub(c_role, in, out);
                System.out.println("対戦相手が「言ったら負けの数」を決定しています......\n");
                //サーバーから決定した数値を受け取り(A-4)
                int d_num;
                d_num = Integer.parseInt(in.readLine());
                newgame.D_Num = d_num;
                System.out.printf("----------言ったら負けの数は%dにきまりました----------\n\n",d_num);
            }


            Integer last=1, nextcount, checkwinner;
            while(newgame.gamecontinue==1){
                
                if(newgame.player2.decideorder==0)
                {   //(A-9) 受け取り(2通りの発信源)
                    checkwinner = Integer.parseInt(in.readLine());
                    if(checkwinner!=1){
                        newgame.gamecontinue=0;
                        newgame.AnnounceWinner(Integer.parseInt(in.readLine()), newgame.player2);
                        continue;
                    }
                    //受け取り(A-6)
                    newgame.PrintNum_opn(Integer.parseInt(in.readLine()), Integer.parseInt(in.readLine()));
                    
                }
                newgame.player2.decideorder = 0;
                
                if(in.readLine().equals("client turn")){
                    last = Integer.parseInt(in.readLine());
                    nextcount = newgame.Turn(last);
                    last = newgame.PrintNum_me(1, nextcount, last, out);
                    //次に呼ばれる数字, 進めた数字の数をサーバーに送信(A-5)
                    out.println(last);
                    out.println(nextcount);
                }
            }

        } finally {
        System.out.println("closing...");
        socket.close();
        }   
    }
}
