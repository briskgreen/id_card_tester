#include <gtk/gtk.h>
#include <string.h>
/*void delete_quit(GtkWidget *message_dialog)
{
	gtk_widget_hide_on_delete(message_dialog);
	gtk_main_quit();
}*/
void about(void)
{
	GtkWidget *win;
	GtkWidget *notenook;
	GtkWidget *page;
	GtkWidget *vbox;
	GtkWidget *label;
	GtkWidget *button;
	win=gtk_window_new(GTK_WINDOW_POPUP);
	gtk_window_set_title(GTK_WINDOW(win),"关于本工具");
	gtk_window_set_position(GTK_WINDOW(win),GTK_WIN_POS_CENTER);
	vbox=gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(win),vbox);
	notenook=gtk_notebook_new();
	page=gtk_vbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox),notenook,TRUE,TRUE,0);
	label=gtk_label_new("使用方法");
	gtk_notebook_append_page(GTK_NOTEBOOK(notenook),page,label);
	label=gtk_label_new("点击\"点击开始使用\"开始使用本程序\n输入身份证号然后点击\"点击进行测试\"");
	gtk_box_pack_start(GTK_BOX(page),label,FALSE,FALSE,5);
	page=gtk_vbox_new(FALSE,0);
	label=gtk_label_new("作者");
	gtk_notebook_append_page(GTK_NOTEBOOK(notenook),page,label);
	label=gtk_label_new("该工具由炕头哥制作\nE-Mail:briskgreen@163.com");
	gtk_box_pack_start(GTK_BOX(page),label,FALSE,FALSE,5);
	button=gtk_button_new_with_label("点击开始使用");
	gtk_box_pack_start(GTK_BOX(vbox),button,FALSE,FALSE,5);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(gtk_main_quit),NULL);
	gtk_widget_show_all(win);
	gtk_main();
	gtk_widget_destroy(win);
}
void error_quit(const char *msg)
{
	GtkWidget *message_dialog;
	message_dialog=gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"%s",msg);
	/*g_signal_connect(G_OBJECT(message_dialog),"destroy",G_CALLBACK(delete_quit),NULL);
	g_signal_connect(G_OBJECT(message_dialog),"response",G_CALLBACK(delete_quit),NULL);
	gtk_widget_show(message_dialog);
	gtk_main();*/
	gtk_dialog_run(GTK_DIALOG(message_dialog));
	gtk_widget_destroy(message_dialog);
}
int get_id(int *card_id,char *id)
{
	int i;
	if(strlen(id)!=18)
	{
		error_quit("错误的身份证号码!");
		return 1;
	}
	for(i=0;id[i];i++)
	{
		if(id[i]=='X'||id[i]=='x')
		{
			card_id[i]=10;
			continue;
		}
		card_id[i]=id[i]-48;
	}
	return 0;
}
int my_return_if_fail(int *card_id,char *id)
{
	if(strlen(id)!=18)
	{
		error_quit("参数错误,请检查后重新输入!");
		return 1;
	}
	int n=18;
	int i;
	for(i=0;i<17;i++)
		if(card_id[i]<0||card_id[i]>9)
		{
			error_quit("身份证号输入有误,请重新输入!");
			return 1;
		}
	if(card_id[i]>10)
	{
		error_quit("身份证号码输入有误,请重新输入!");
		return 1;
	}
	return 0;
}
int test_id(int *card_id,int *mask)
{
	int i;
	int result=0;
	for(i=0;i<17;i++)
		result+=card_id[i]*mask[i];
	return result;
}
void message_report(char *id,int result,int flags)
{
	GtkWidget *message_dialog;
	if(flags)
		message_dialog=gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"身份证号:%s是正确的!",id);
	else
		message_dialog=gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"身份证号:%s是错误的!\n最后一位应该是%d!",id,result);
/*	g_signal_connect(G_OBJECT(message_dialog),"destroy",G_CALLBACK(delete_quit),NULL);
	g_signal_connect(G_OBJECT(message_dialog),"response",G_CALLBACK(delete_quit),NULL);
	gtk_widget_show(message_dialog);
	gtk_main();*/
	gtk_dialog_run(GTK_DIALOG(message_dialog));
	gtk_widget_destroy(message_dialog);
}
void get_card_id_and_test(GtkWidget *button,GtkWidget *entry)
{
	int card_id[18];
	int mask[17]={7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2};
	int res[11]={1,0,10,9,8,7,6,5,4,3,2};
	int result;
	char *id;
	id=(char *)gtk_entry_get_text(GTK_ENTRY(entry));
	if(get_id(card_id,id))
		return;
	if(my_return_if_fail(card_id,id))
		return;
	result=test_id(card_id,mask);
	result%=11;
	if(res[result]==card_id[17])
		message_report(id,res[result],1);
	else
		message_report(id,res[result],0);
}
int main(int argc,char **argv)
{
	GtkWidget *win;
	GtkWidget *hbox;
	GtkWidget *button;
	GtkWidget *entry;
	gtk_init(&argc,&argv);
	about();
	win=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win),"ID Card Tester");
	g_signal_connect(G_OBJECT(win),"destroy",G_CALLBACK(gtk_main_quit),NULL);
	gtk_window_set_position(GTK_WINDOW(win),GTK_WIN_POS_CENTER);
	hbox=gtk_hbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(win),hbox);
	button=gtk_button_new_with_label("点击进行测试");
	entry=gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox),entry,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(hbox),button,FALSE,FALSE,5);
	gtk_entry_set_text(GTK_ENTRY(entry),"请输入身份证号码");
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(get_card_id_and_test),entry);
	gtk_widget_show_all(win);
	gtk_main();
	return 0;
}
