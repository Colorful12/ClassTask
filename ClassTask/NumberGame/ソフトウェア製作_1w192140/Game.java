import java.util.Random;
import java.util.Scanner;
import java.rmi.server.ServerCloneException;
import java.util.InputMismatchException;

import java.io.*;
import java.net.*;



public class Game {
    static Player player1;
    static Player player2;
    Integer D_Num, C_Num, turn, gamecontinue = 1;
    //turn : 1はサーバーのターン, 0はクライアントのターン
    static Integer gonext; 

    public Game(){
        player1 = new Player();
        player1.server = 1;
        player2 = new Player();
        player2.server = 0;
        gonext = 1;
    }

    public void Rule(){
        try{
            System.out.println("数とりゲームを開始します。\n\n");
            Thread.sleep(1000);
            System.out.println("--------------------ルール説明--------------------\n");
            Thread.sleep(2000);
            System.out.println("数とりゲームは、簡単に言えば数字をカウントしていくだけのゲームです。\n");
            Thread.sleep(2000);
            System.out.println("しかし、言ったら負けの数字を相手に押し付けられるように考える必要があります。\n");
            Thread.sleep(2000);
            System.out.println("たとえば、「1度に進められる数字の数」が3だとし、「言ったら負けの数」が10だとします。\n");
            Thread.sleep(2000);
            System.out.println("このルールでAさんとBさんがゲームをしている様子をみてみましょう！");
            Thread.sleep(2000);
            System.out.println("A 『1』");
            Thread.sleep(2000);
            System.out.println("B 『2,3』");
            Thread.sleep(1000);
            System.out.println("A 『4,5』");
            Thread.sleep(2000);
            System.out.println("B 『6』");
            Thread.sleep(500);
            System.out.println("A 『7,8,9』");
            Thread.sleep(3000);
            System.out.println("B 『...10』\n");
            Thread.sleep(2000);
            System.out.println("Bさんが「言ったら負けの数」の10を言ってしまいましたね。つまりAさんの勝ちです。\n");
            Thread.sleep(2000);
            System.out.println("それでは実際にやってみましょう！\n");
            Thread.sleep(2000);
            System.out.println("まず、先攻と後攻を決めます。ランダムに決まるのでご安心を。\n");
            Thread.sleep(2000);
            System.out.println("次に、先攻の方には「一度に進められる数字の数」を、後攻の方には「言ったら負けの数」を決定していただきます。\n");
            Thread.sleep(2000);
            System.out.println("それが終わったらゲームスタート！先攻の方から「進める数字の数」を入力していっていただきます。\n");
            Thread.sleep(2000);
            System.out.println("ルールはわかりましたか？あなたと対戦相手の準備ができたらゲームを開始します。\n");
            Thread.sleep(2000);
            System.out.println("-----------準備ができたらキーボードで1を入力してください----------\n");
        }catch (InterruptedException e) {}
    }

    public void GameStart_Server(BufferedReader in, PrintWriter out){
        Random rand = new Random();
        System.out.println("\n順番を決定します。\n");
        player1.decideorder = rand.nextInt(2);
        player2.decideorder = Math.abs(player1.decideorder - 1);
        //Clientにdecideorderを送信
        out.println(player2.decideorder);
        turn = player1.decideorder;
        try{
            Thread.sleep(2000);
        }catch (InterruptedException e) {}
        if(player1.decideorder==1)System.out.println("あなたは先攻です。");
        else System.out.println("あなたは後攻です。");
    }

    public void Gamestart_Client(BufferedReader in, PrintWriter out)throws IOException{
        System.out.println("\n順番を決定します。\n");
        try{
            Thread.sleep(2000);
        }catch (InterruptedException e) {}
        int order;
        order = Integer.parseInt(in.readLine());
        //クライアントが初回のときに Game.Client L55の命令を初回だけ実行しないようにするためにdecideorderを利用する
        //よって、Serveの方の値をうけとり格納
        player2.decideorder=order;
        player1.decideorder = Math.abs(player2.decideorder - 1);
        if(order==1)System.out.println("あなたは先攻です。");
        else System.out.println("あなたは後攻です。");
    }

    public void DecideRules_sub(String whattodo, BufferedReader in, PrintWriter out)throws IOException{
        int c_num, d_num;
        Scanner scan = new Scanner(System.in);
        if(whattodo.equals("CountNum")){
            System.out.println("一度に進められる数字の数を入力してください.3~5がおすすめです");
            c_num = Math.round(scan.nextFloat());
            c_num = Math.abs(c_num);
            C_Num = c_num;
            System.out.printf("----------度に進められる数は%dになりました----------\n\n", c_num);
            //サーバーに決定した数値を送る(A-1)
            out.println(c_num);
        }else if(whattodo.equals("DangerousNum")){
            System.out.println("言ったら負けの数を入力してください.30以上がおすすめです");
            d_num = Math.round(scan.nextFloat());
            d_num = Math.abs(d_num);
            D_Num = d_num;
            System.out.printf("----------言ったら負けの数は%dにきまりました----------\n\n", d_num);
            //サーバーに決定した数値を送信(A-2)
            out.println(d_num);
        }else{
            if(player1.decideorder==0){
                //追加した！
                out.println("DangerousNum");
                DecideRules(player1, in, out);
                System.out.println("対戦相手が「言ったら負けの数」を決定しています......\n");
                DecideRules(player2, in, out);
            }else{
                System.out.println("対戦相手が「一度に進められる数字の数」を決定しています......\n");
                DecideRules(player2, in, out);
                 //クライアントからの決定した数値を受け取る(A-1)
                c_num = Integer.parseInt(in.readLine());
                C_Num = c_num;
                System.out.printf("----------一度に進められる数字の数は%dになりました----------\n\n", c_num);
                DecideRules(player1, in, out);
            }
        }
    }

    public void DecideRules(Player player, BufferedReader in, PrintWriter out)throws IOException{
        if(player.decideorder==1){
            DangerousNum(player.server, in, out);
        }else{
            CountNum(player.server, in, out);
        }
    }

    

    public void DangerousNum(int server, BufferedReader in, PrintWriter out)throws IOException{
        int d_num;
        if(server==1){
            Scanner scan = new Scanner(System.in);
            System.out.println("言ったら負けの数を入力してください。30以上がおすすめです。");
            d_num = Math.round(scan.nextFloat());
            d_num = Math.abs(d_num);
            D_Num = d_num;
            //クライアントに決定した数値を送信(A-4)
            out.println(d_num);
            System.out.printf("----------一度に進められる数字の数は%dになりました----------\n\n", d_num);
        }else{}
    }

    public void CountNum(int server, BufferedReader in, PrintWriter out) throws IOException{
        int c_num;
        if(server==1){
            Scanner scan = new Scanner(System.in);
            System.out.println("一度に進められる数字の数を入力してください。3~5がおすすめです。");
            c_num = Math.round(scan.nextFloat());
            c_num = Math.abs(c_num);

            C_Num = c_num;
            out.println(c_num);
            System.out.printf("----------一度に進められる数字の数は%dになりました----------\n\n", c_num);
        }else{  
                //クライアントに役割を送信(A-3)
                out.println("CountNum");
            }
        }
    
    public Integer Turn_sub(Integer last, BufferedReader in, PrintWriter out){
        int nextcount;
        if(turn==1)nextcount = Turn(last);
        else {
            SendClient(last, in, out);
            nextcount = -100;
        }
        turn = Math.abs(turn-1);
        return nextcount;
    }

    public Integer Turn(Integer last){
        int nextcount=0, ok=0;
        Scanner scan = new Scanner(System.in);
        if(last -1 + C_Num > D_Num) C_Num = D_Num-(last-1);
        System.out.printf("----------次の数は%dです----------\n\n",last);
        System.out.printf("あなたの番です。\n");
        if(C_Num==1){
            System.out.printf("残念ながら、%dしか入力できません。覚悟が決まったら%dを入力してください。\n", C_Num, C_Num);
        }else System.out.printf("いくつ数字を進めますか？  1～%dから選んで入力してください。\n", C_Num);
        while(ok==0){
            nextcount = scan.nextInt();
            if(nextcount<1 || nextcount >C_Num){
                System.out.println("!!!!!!!!!!入力エラー!!!!!!!!!!");
                if(C_Num==1)System.out.printf("1しか入力できません。\n",C_Num);
                else System.out.printf("1から%dまでの数値を入力してください。\n",C_Num);
            }else ok=1;
        }

        System.out.println("\n----------------------------------");
        return nextcount;
       }
    
    public void SendClient(Integer last, BufferedReader in, PrintWriter out){
        out.println("client turn");
        out.println(last);
    }

    //このclientいらないかも
    public Integer PrintNum_me(Integer client, Integer num, Integer last, PrintWriter out){
        System.out.printf("あなた「 ");
        for(Integer i=0;i<num;i++){
            System.out.printf("%d ",last);
            last++;
        }
        System.out.println("」");

        return last;
    }

    public void PrintNum_opn(Integer last, Integer countnum){
        System.out.print("あいて「 ");
        for(Integer i=countnum;i>0;i--){
            System.out.printf("%d ", last-countnum);
            last++;
        }
        System.out.println("」");
    }

    public Integer CheckWinner(Integer last, PrintWriter out){
        if(last-1 >= D_Num){
            return turn;
        }else return 100;
    }

    public void AnnounceWinner(int winner, Player player){
        gamecontinue=0;
        if(winner==player.server){
            Winner();
        }else Loser();
    }

    public void Winner(){
        System.out.println("--------------------YOU WIN--------------------\n\n");
        System.out.println("おめでとうございます！あなたの勝利です！\n");
        SendEnding();
    }

    public void Loser(){
        System.out.println("--------------------YOU LOSE--------------------\n\n");
        System.out.println("対戦相手の勝利です！\n");
        SendEnding();
    }

    public void SendEnding(){
        try{
            Thread.sleep(1000);
        }catch (InterruptedException e) {}
        System.out.println("---------------ゲームを終了します---------------\n\n");
    }
}
