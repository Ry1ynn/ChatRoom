﻿// CChatZone.cpp: 实现文件
//

#include "stdafx.h"
#include "ChatRoomClient.h"
#include "CChatDlg.h"
#include "afxdialogex.h"
#include "client.h"
#include "ChatRoomClientDlg.h"


// CChatZone 对话框

IMPLEMENT_DYNAMIC(CChatDlg, CDialogEx)

CChatDlg::CChatDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CHATZONE, pParent)
	
{

}

CChatDlg::~CChatDlg()
{
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FINDFRIEND, m_EditFindFriend);
	//  DDX_Text(pDX, IDC_STATIC_CURUSER, m_currentAccount);
	DDX_Control(pDX, IDC_EDIT_CHAT, m_EditChatInput);
	DDX_Control(pDX, IDC_EDIT_RECORD, m_EditChatRecord);
	DDX_Control(pDX, IDC_EDIT_CURUSR, m_curentAccount);
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CChatDlg::OnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN, &CChatDlg::OnClickedButtonClean)
//	ON_EN_UPDATE(IDC_EDIT_CHAT, &CChatZone::OnUpdateEditChat)
//ON_WM_TIMER()
END_MESSAGE_MAP()


// CChatZone 消息处理程序


void CChatDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	// 关闭聊天框后再显示登录框
	AfxGetApp()->GetMainWnd()->ShowWindow(SW_NORMAL);
	//g_isLogin = false;
	//CWnd* phWnd = AfxGetApp()->GetMainWnd(); //获取主窗口的句柄
	//ShowWindow(SW_NORMAL);// 隐藏主窗口（当关闭会话框时再显示

	CDialogEx::OnClose();
}


BOOL CChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 获取本窗口句柄
	g_hpWndChat = this->m_hWnd;

	// 获取聊天记录框的控件句柄
	g_hpEditChatRecord = (CEdit*)GetDlgItem(IDC_EDIT_RECORD);


	// 搜索好友编辑框的默认值
	CEdit*  pEditFindFri = (CEdit*)GetDlgItem(IDC_EDIT_FINDFRIEND);//获取相应的编辑框ID
	pEditFindFri->SetWindowText(_T("请输入好友姓名")); //设置默认显示的内容 

	// 新开一个线程，接收消息
	CreateThread(0, 0, recvMessageProc, &g_pClient, 0, 0);

	// 显示当前账号
	CEdit*  pEditCurUsr = (CEdit*)GetDlgItem(IDC_EDIT_CURUSR);//获取相应的编辑框ID
	pEditCurUsr->SetWindowText(g_CurAccount);
	/*UpdateData(TRUE);
	char buff[30];
	sprintf_s(buff, " %s", g_CurAccount);
	m_currentAccount = buff;
	UpdateData(FALSE);
*/



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CChatDlg::OnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码

	// 获取发送的消息内容
	CString message;
	m_EditChatInput.GetWindowTextW(message);
	// 判断是否为空
	if (message.IsEmpty())
		MessageBox(L"什么都不写，你发nima呢");
	else
	{
		//MessageBox(CString("已发送: ") + message);

		// cstring 转const char *
		size_t  i;
		const wchar_t* wstr = (LPCTSTR)message;
		char smessage[20] = { 0 };
		setlocale(LC_ALL, "chs");
		wcstombs_s(&i, smessage, wstr, wcslen(wstr));
		const char * ssmessage = smessage;//不加则插入数据库失败
		setlocale(LC_ALL, "C");
		// 发送消息
		SendMsg(g_pClient, ssmessage);


		// 显示在聊天记录
		CEdit*  pEditRecord = (CEdit*)GetDlgItem(IDC_EDIT_RECORD);
		// 获取时间
		CString strTime; 
		CString oldRecord;
		CTime tm;
		tm = CTime::GetCurrentTime();
		strTime = tm.Format("%Y-%m-%d %X   ");
		// 新旧内容结合后显示
		pEditRecord->GetWindowText(oldRecord);
		pEditRecord->SetWindowText(oldRecord +"\r\n"+ strTime + "Send:   " + message);

	}
		
}


void CChatDlg::OnClickedButtonClean()
{
	// TODO: 在此添加控件通知处理程序代码

	// 清空消息
	CEdit*  pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CHAT);//获取相应的编辑框ID
	pEdit->SetWindowText(_T("")); //设置默认显示的内容 

}
