#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node{
    // kuyrukdali nodelarin yapisi

    int oncelik_Id;
    int ucak_Id;
    int talep_edilen_inis_saati;
    int inis_saati;
    int kalkis_saati;
    int rotarSayisi;

    struct node* sonrakiNode;

};
struct node* root_inis;
struct node* root_kalkis;
struct node* temp_kalkis;
FILE* dosyaIn;
FILE* dosyaOut;

int *kalkislar[6];
int kayitAdet = 0;

void add(struct node** root ,struct node* eklenecek_node){
    // Kuyrugun basina eleman ekler

    if(*root == NULL){
        *root = eklenecek_node;
        return;
    }
    eklenecek_node->sonrakiNode = *root;
    *root = eklenecek_node;
}

void rearAdd(struct node** root, struct node* eklenecek_node){
    // Kuyrugun sonuna eleman ekler
    if(*root == NULL){
        *root = eklenecek_node;
        return;
    }
    struct node* tempNode = *root;
    while(tempNode->sonrakiNode != NULL){
        tempNode = tempNode->sonrakiNode;
    }
    tempNode->sonrakiNode = eklenecek_node;
    return;
}

void addBetween(struct node** dugum, struct node* eklenecek_node){
    // dugumun sonraki node'nu eklenecek node yapar
    if(*dugum == NULL){
        *dugum = eklenecek_node;
        return;
    }
    eklenecek_node->sonrakiNode = (*dugum)->sonrakiNode;
    (*dugum)->sonrakiNode = eklenecek_node;
    return;
}


struct node* poll(struct node** root ){
    // Kuyruktaki son elemaný dondurur ve kuyruktan siler
    struct node* tempNode = *root;
    while(tempNode->sonrakiNode->sonrakiNode != NULL){
        tempNode = tempNode->sonrakiNode;
    }
    //sondan bir onceki node
    struct node* sonNode = tempNode->sonrakiNode;
    tempNode->sonrakiNode = NULL;
    return sonNode;
}

struct node* peek(struct node** root ){

    // Kuyruktaki son elemaný dondurur
    struct node* tempNode = *root;
    while(tempNode->sonrakiNode != NULL){
        tempNode = tempNode->sonrakiNode;
    }
    return tempNode;
}

void queue_clear(struct node** root ){

    // Kuyruktaki tum elemanlari siler
    struct node* sonraki;
    while(*root != NULL){
        sonraki = (*root)->sonrakiNode;
        free(*root);
        *root = sonraki;
    }
}

void queue_remove(struct node** root ,int ucak_Id){

    // Kuyruktaki belirtilen elemani siler
    struct node* tempNode = *root;
    while(tempNode->sonrakiNode->ucak_Id != ucak_Id){
        tempNode = tempNode->sonrakiNode;
    }
    // tempNode sonraki nodu istenilen node
    struct node* silinecek_node = tempNode->sonrakiNode;
    tempNode->sonrakiNode = silinecek_node->sonrakiNode;
    free(silinecek_node);
}


void kuyrugu_Goster(struct node** root ){
    // Kuytuktaki tum nodelari gosterir
    struct node* tempNode = *root;
    while(tempNode != NULL){
        printf("\n%d oncelikli %d numarali ucagin talep edilen inis saati: %d:00 kalkis saati: %d:00",tempNode->oncelik_Id,tempNode->ucak_Id,tempNode->talep_edilen_inis_saati,tempNode->kalkis_saati);
        tempNode = tempNode->sonrakiNode;
    }
}


int dosyaKontrol(char dosyaAdi[],int in){
    if(in){
        dosyaIn = fopen(dosyaAdi,"r");
        if(dosyaIn != NULL){
            //printf("\nDosya acildi");
            return 1;
        }
        else{
            printf("\nDosya mevcut degil");
            return 0;
        }
    }

    else{
        dosyaOut = fopen(dosyaAdi,"r");
        if(dosyaOut != NULL){
            //printf("\nDosya acildi");
            return 1;
        }
        else{
            printf("\nDosya mevcut degil");
            return 0;
        }
    }



}



int siraBul(struct node** root, struct node* aranan){
    // aranan node'un indexini dondurur bulamazsa -1 doner
    int sira = 0;
    struct node* temp_root = *root;
    while(temp_root != NULL){
        if(temp_root == aranan)
            break;
        sira++;
        temp_root = temp_root->sonrakiNode;
    }
    if(temp_root == NULL)
        sira = -1;

    return sira;
}



void dosyaYaz(){

    FILE* outputFile = fopen("output.txt","w");

    struct node* tempNode = root_kalkis;
    while(tempNode != NULL){
        fprintf(outputFile,"%d %d %d %d %d %d\n",tempNode->oncelik_Id,tempNode->ucak_Id,tempNode->talep_edilen_inis_saati,tempNode->inis_saati,tempNode->rotarSayisi,tempNode->kalkis_saati);
        tempNode = tempNode->sonrakiNode;
    }
    fclose(outputFile);
}

void yenidosyaYaz(){

    FILE* outputFile = fopen("output.txt","w");
    for(int i = 0; i < kayitAdet; i++)
        fprintf(outputFile,"%d %d %d %d %d %d\n",kalkislar[i][0],kalkislar[i][1],kalkislar[i][2],kalkislar[i][3],kalkislar[i][4],kalkislar[i][5]);
    fclose(outputFile);
}

void listeyeKayitOlustur(int oncelikId, int ucakId, int talepEdilenSaat, int inisSaat, int gecikme, int kalkis){

    kayitAdet = kayitAdet + 1;
    // okunan kalkis listesini dizinin degerlerine ata
    kalkislar[kayitAdet-1] = (int)malloc(sizeof(int)*6);
    kalkislar[kayitAdet-1][0] = oncelikId;
    kalkislar[kayitAdet-1][1] = ucakId;
    kalkislar[kayitAdet-1][2] = talepEdilenSaat;
    kalkislar[kayitAdet-1][3] = inisSaat;
    kalkislar[kayitAdet-1][4] = gecikme;
    kalkislar[kayitAdet-1][5] = kalkis;
}

void kalkisDosyasiniOku(){
    kayitAdet = 0;
    if(dosyaKontrol("output.txt",0)){
        int satirUzunlugu = 256;
        char *tempSatir = (char *)malloc(sizeof(char) * satirUzunlugu);
        char karakter = getc(dosyaOut);
        int adet = 0;
        while(karakter != EOF){
            if(karakter == '\n'){
                tempSatir[adet] = '\0';
                // satir okundu bosluga gore ayir
                char* ayir = strtok(tempSatir," ");
                int oncelikId = -1, ucakId = -1,talep_edlien_saat = -1, inisSaati = -1,gecikme = -1, kalkis = -1;
                while(ayir != NULL){
                    if(oncelikId == -1)
                        oncelikId = atoi(ayir);
                    else if(ucakId == -1)
                        ucakId = atoi(ayir);
                    else if(talep_edlien_saat == -1)
                        talep_edlien_saat = atoi(ayir);
                    else if(inisSaati == -1){
                        inisSaati = atoi(ayir);
                    }
                    else if(gecikme == -1){
                        gecikme = atoi(ayir);
                    }
                    else if(kalkis == -1){
                        kalkis = atoi(ayir);
                    }
                    ayir = strtok(NULL," ");
                }
                // yeni satir okuma icin adeti sifirla
                adet = 0;


                // kalkislar listesine kayit olustur

                listeyeKayitOlustur(oncelikId,ucakId,talep_edlien_saat,inisSaati,gecikme,kalkis);
            }
            else{
                tempSatir[adet++] = karakter;
            }
            karakter = getc(dosyaOut);
        }
    }
}

void listedeYerDegistir(int fromIndex, int toIndex){

    int* tempKayit = (int*)malloc(sizeof(int)*6);

    tempKayit[0] = kalkislar[fromIndex][0];
    tempKayit[1] = kalkislar[fromIndex][1];
    tempKayit[2] = kalkislar[fromIndex][2];
    tempKayit[3] = kalkislar[fromIndex][3];
    tempKayit[4] = kalkislar[fromIndex][4];
    tempKayit[5] = kalkislar[fromIndex][5];

    kalkislar[fromIndex][0] = kalkislar[toIndex][0];
    kalkislar[fromIndex][1] = kalkislar[toIndex][1];
    kalkislar[fromIndex][2] = kalkislar[toIndex][2];
    kalkislar[fromIndex][3] = kalkislar[toIndex][3];
    kalkislar[fromIndex][4] = kalkislar[toIndex][4];
    kalkislar[fromIndex][5] = kalkislar[toIndex][5];

    kalkislar[toIndex][0] = tempKayit[0];
    kalkislar[toIndex][1] = tempKayit[1];
    kalkislar[toIndex][2] = tempKayit[2];
    kalkislar[toIndex][3] = tempKayit[3];
    kalkislar[toIndex][4] = tempKayit[4];
    kalkislar[toIndex][5] = tempKayit[5];

}

void yeniRotarla(int index){
    // gelen indexi bir oncekine göre rötarlar
    if(index == kayitAdet)
        return;
    if(kalkislar[index][3] == kalkislar[index-1][3]){
        if(kalkislar[index][4] == 3){
            printf("%d idli ucak daha falza rotar yapamaz.\n",kalkislar[1]);
            return;
        }
        else{
            kalkislar[index][3] = kalkislar[index][3] + 1;
            kalkislar[index][4] = kalkislar[index][4] + 1;
            kalkislar[index][5] = kalkislar[index][5] + 1;
            yeniRotarla(index+1);
        }
    }
    else{
        yeniRotarla(index+1);
    }

}

void listeyeYerlestir(struct node** eklenecekUcak){

    int yerlestirildi = 0;
    // kalkis listesini gez
    for(int i = 0; i < kayitAdet; i++){
        if((*eklenecekUcak)->rotarSayisi > 3){
            printf("%d idli ucak yogunluk nedeniyle inis izni verilemedi.\n",(*eklenecekUcak)->ucak_Id);
            yerlestirildi = 1;
            break;
        }
        if(yerlestirildi)
            break;
        //listedeki ucagin rotar sayisina bak
        if(kalkislar[i][4] == 3){
            // ucak daha fazla rotar yapamaz
            printf("%d numarali ucak daha fazla rotar yapamaz.\n",kalkislar[i][1]);
            continue;
        }
        else{
            // ucak rotar yapabiliyorsa
            // gelen ucak ile listedeki ucagin inis saatlerine bak
            if(kalkislar[i][3] < (*eklenecekUcak)->inis_saati){
                // eger listedeki ucak yeni gelen ucaktan once inecek ise listedeki ucagi atla
                continue;
            }
            else if(kalkislar[i][3] == (*eklenecekUcak)->inis_saati){
                // eger listedeki ucak yeni gelen ucakla ayni saatte inecek ise listedeki ucakla yeni gelen ucagi oncelik olarak karsilastir.
                if(kalkislar[i][0] <= (*eklenecekUcak)->oncelik_Id){
                    // listedeki ucak yeni gelen ucaktan daha fazla oncelikli veya esit ise yeni ucagi listede koru mevcut ucaga 1 saat rotar yaptir
                    (*eklenecekUcak)->inis_saati = (*eklenecekUcak)->inis_saati + 1;
                    (*eklenecekUcak)->rotarSayisi = (*eklenecekUcak)->rotarSayisi + 1;
                    (*eklenecekUcak)->kalkis_saati = (*eklenecekUcak)->kalkis_saati + 1;
                    continue;
                }
                else if(kalkislar[i][0] > (*eklenecekUcak)->oncelik_Id){
                    // listedeki ucak yeni gelen ucaktan daha az oncelikli ise yeni ucagi listeye ekle listenin kalanini 1 saat rotarla
                    if(kalkislar[i][4] == 3){
                        (*eklenecekUcak)->inis_saati = (*eklenecekUcak)->inis_saati + 1;
                            (*eklenecekUcak)->rotarSayisi = (*eklenecekUcak)->rotarSayisi + 1;
                            (*eklenecekUcak)->kalkis_saati = (*eklenecekUcak)->kalkis_saati + 1;
                            continue;
                    }
                    else{
                        listeyeKayitOlustur((*eklenecekUcak)->oncelik_Id,(*eklenecekUcak)->ucak_Id,(*eklenecekUcak)->talep_edilen_inis_saati,(*eklenecekUcak)->inis_saati,(*eklenecekUcak)->rotarSayisi,(*eklenecekUcak)->kalkis_saati);
                        yerlestirildi = 1;
                        printf("%d idli ucak icin inis izni verilmistir.\n",(*eklenecekUcak)->ucak_Id);
                        listedeYerDegistir(kayitAdet-1,i);
                        for(int j = i+1; j < kayitAdet; j++)
                            if(j != kayitAdet-1)
                                listedeYerDegistir(j,kayitAdet-1);

                        yeniRotarla(i+1);
                    }
                    break;
                }

            }
            else if(kalkislar[i][3] > (*eklenecekUcak)->talep_edilen_inis_saati){
                // eger listedeki ucak yeni gelen ucaktan sonra inecek ise yeni gelen ucagi ekle ve listenin kalanini aynen ekle
                listeyeKayitOlustur((*eklenecekUcak)->oncelik_Id,(*eklenecekUcak)->ucak_Id,(*eklenecekUcak)->talep_edilen_inis_saati,(*eklenecekUcak)->inis_saati,(*eklenecekUcak)->rotarSayisi,(*eklenecekUcak)->kalkis_saati);
                yerlestirildi = 1;
                printf("%d idli ucak icin inis izni verilmistir.\n",(*eklenecekUcak)->ucak_Id);
                // yeni gelen ucagi listenin i. konumua tasi ve listenin kalanini 1 ile kaydir.
                listedeYerDegistir(kayitAdet-1,i);
                for(int j = i+1; j < kayitAdet; j++)
                    if(j != kayitAdet-1)
                        listedeYerDegistir(j,kayitAdet-1);
                break;

            }
        }

    }
    if(!yerlestirildi){
        // ucak en son siraya alindi
        listeyeKayitOlustur((*eklenecekUcak)->oncelik_Id,(*eklenecekUcak)->ucak_Id,(*eklenecekUcak)->talep_edilen_inis_saati,(*eklenecekUcak)->inis_saati,(*eklenecekUcak)->rotarSayisi,(*eklenecekUcak)->kalkis_saati);
        yerlestirildi = 1;
        printf("%d idli ucak icin inis izni verilmistir.\n",(*eklenecekUcak)->ucak_Id);
    }


}


int main()
{
    printf("\tOncelikli Kuyruk Uygulamasi\n");
    struct node* yeniNode;
    struct node* sonNode;
    int secim = 0;
    int silincekUcakId;
    yenidosyaYaz();

    if(dosyaKontrol("input.txt",1)){
        // Satir sonuna kadar okuma
        int satirUzunlugu = 256;
        char *tempSatir = (char *)malloc(sizeof(char) * satirUzunlugu);
        char karakter = getc(dosyaIn);
        int adet = 0;
        while(karakter != EOF){
            if(karakter == '\n'){
                tempSatir[adet] = '\0';
                // satir okundu bosluga gore ayir
                char* ayir = strtok(tempSatir," ");
                int oncelikId = -1, ucakId = -1, inisSaati = -1;
                while(ayir != NULL){
                    if(oncelikId == -1)
                        oncelikId = atoi(ayir);
                    else if(ucakId == -1)
                        ucakId = atoi(ayir);
                    else if(inisSaati == -1)
                        inisSaati = atoi(ayir);
                    ayir = strtok(NULL," ");
                }

                // Yeni kayit alindi node olustur
                yeniNode = (struct node*)malloc(sizeof(struct node));
                yeniNode->oncelik_Id = oncelikId;
                yeniNode->ucak_Id = ucakId;
                yeniNode->talep_edilen_inis_saati = inisSaati;
                yeniNode->inis_saati = inisSaati;
                yeniNode->kalkis_saati = inisSaati + 1;
                yeniNode->rotarSayisi = 0;
                yeniNode->sonrakiNode = NULL;

                //printf("\nYeni node eklendi.");
                adet = 0;

                // daha once olusturulmus kalkis listesini dosyadan cek ve kalkislar dizisine ata
                //system("CLS");
                kalkisDosyasiniOku();

                // yeni gelen ucagi listeye yerlestir

                if(kayitAdet)
                    listeyeYerlestir(&yeniNode);
                else{
                    listeyeKayitOlustur(yeniNode->oncelik_Id,yeniNode->ucak_Id,yeniNode->talep_edilen_inis_saati,yeniNode->inis_saati,yeniNode->rotarSayisi,yeniNode->kalkis_saati);
                    printf("%d idli ucak icin inis izni verilmistir.\n",yeniNode->ucak_Id);
                }

                // yeni listeyi dosyaya yaz
                yenidosyaYaz();
                //system("PAUSE");
            }
            else{
                tempSatir[adet++] = karakter;
            }
            karakter = getc(dosyaIn);
        }
    }
    /*
    do{
        printf("\n------------------------------------------------\n");
        printf("\t[1] Kuyruga eleman ekle\n");
        printf("\t[2] Kuyruktaki son elemani goster ve kuyruktan sil\n");
        printf("\t[3] Kuyruktaki son elemani goster\n");
        printf("\t[4] Kuyruktaki belirtilen elemani sil\n");
        printf("\t[5] Kuyrugu temizle\n");
        printf("\t[6] Kuyrugu goster\n");
        printf("\t[0] Cikis Yap");
        printf("\n------------------------------------------------\n");
        printf("Lutfen secim yapiniz: ");
        scanf("%d",&secim);
        switch(secim){
            case 1:
                yeniNode = (struct node*)malloc(sizeof(struct node));

                printf("\n\tOncelik Id: "); scanf("%d",&yeniNode->oncelik_Id);
                printf("\n\tUcak Id: "); scanf("%d",&yeniNode->ucak_Id);
                printf("\n\tTalep Edilen Inis Saati: "); scanf("%d",&yeniNode->talep_edilen_inis_saati);
                yeniNode->kalkis_saati = yeniNode->talep_edilen_inis_saati + 1;
                yeniNode->rotarSayisi = 0;
                yeniNode->sonrakiNode = NULL;
                add(root_inis,yeniNode);
                printf("\nYeni node eklendi.");
                break;
            case 2:
                sonNode = poll(&root_inis);
                printf("\nSilinen son node \n\tOncelik Id: %d\n\tUcak Id: %d\n\tTalep Edilen Inis Saati: %d",sonNode->oncelik_Id,sonNode->ucak_Id,sonNode->talep_edilen_inis_saati);
                free(sonNode);
                break;
            case 3:
                sonNode = peek(&root_inis);
                printf("\nSon node \n\tOncelik Id: %d\n\tUcak Id: %d\n\tTalep Edilen Inis Saati: %d",sonNode->oncelik_Id,sonNode->ucak_Id,sonNode->talep_edilen_inis_saati);
                break;
            case 4:
                silincekUcakId;
                printf("\nSilinecek ucak id: "); scanf("%d",&silincekUcakId);
                queue_remove(&root_inis,silincekUcakId);
                printf("\n%d idli ucak kuyruktan silindi",silincekUcakId);
                break;
            case 5:
                queue_clear(&root_inis);
                printf("\nKuyruk temizlendi");
                break;
            case 6:
                kuyrugu_Goster(&root_inis);
                break;
            case 0: printf("\nCikis yapiliyor!\n"); break;
            default: printf("\nLutfen geceri bir deger girin!"); break;
        }
        if(secim != 0){
            printf("\n\tYeni islem icin bir tusa basin");
            getch();
        }
    }while(secim != 0);

*/
    system("PAUSE");
    return 0;
}
