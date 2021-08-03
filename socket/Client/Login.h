#include "RegForm.h"
#include"Gold.h"
#include "clientSocket.h"
namespace Client {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class Login : public System::Windows::Forms::Form
	{
	public:
		int socket;
		Login(int sock)
		{
			this->socket = sock;
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Login()
		{
			if (components)
			{
				delete components;
			}
		}
	private: 
		System::Windows::Forms::TextBox^ userbox;
		System::Windows::Forms::TextBox^ passbox;
		System::Windows::Forms::Label^ tktext;
		System::Windows::Forms::Label^ pstext;
		System::Windows::Forms::PictureBox^ pcBox;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::LinkLabel^ linkLabel1;
	private: System::Windows::Forms::Label^ label1;



	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(Login::typeid));
			this->userbox = (gcnew System::Windows::Forms::TextBox());
			this->passbox = (gcnew System::Windows::Forms::TextBox());
			this->tktext = (gcnew System::Windows::Forms::Label());
			this->pstext = (gcnew System::Windows::Forms::Label());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->linkLabel1 = (gcnew System::Windows::Forms::LinkLabel());
			this->label1 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// userbox
			// 
			this->userbox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->userbox->Location = System::Drawing::Point(20, 88);
			this->userbox->Name = L"userbox";
			this->userbox->Size = System::Drawing::Size(233, 22);
			this->userbox->TabIndex = 0;
			this->userbox->TextChanged += gcnew System::EventHandler(this, &Login::userbox_TextChanged);
			// 
			// passbox
			// 
			this->passbox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->passbox->Location = System::Drawing::Point(20, 140);
			this->passbox->Name = L"passbox";
			this->passbox->PasswordChar = '*';
			this->passbox->Size = System::Drawing::Size(233, 22);
			this->passbox->TabIndex = 0;
			this->passbox->TextChanged += gcnew System::EventHandler(this, &Login::passbox_TextChanged);
			// 
			// tktext
			// 
			this->tktext->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->tktext->Location = System::Drawing::Point(20, 66);
			this->tktext->Name = L"tktext";
			this->tktext->Size = System::Drawing::Size(80, 25);
			this->tktext->TabIndex = 1;
			this->tktext->Text = L"Tài khoản:";
			// 
			// pstext
			// 
			this->pstext->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->pstext->Location = System::Drawing::Point(20, 120);
			this->pstext->Name = L"pstext";
			this->pstext->Size = System::Drawing::Size(80, 25);
			this->pstext->TabIndex = 2;
			this->pstext->Text = L"Mật khẩu:";
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(99, 3);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(75, 60);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 3;
			this->pictureBox1->TabStop = false;
			// 
			// button1
			// 
			this->button1->Enabled = false;
			this->button1->Location = System::Drawing::Point(99, 186);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 4;
			this->button1->Text = L"Đăng nhập";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Login::LoginClick);
			// 
			// linkLabel1
			// 
			this->linkLabel1->AutoSize = true;
			this->linkLabel1->Location = System::Drawing::Point(66, 212);
			this->linkLabel1->Name = L"linkLabel1";
			this->linkLabel1->Size = System::Drawing::Size(141, 13);
			this->linkLabel1->TabIndex = 5;
			this->linkLabel1->TabStop = true;
			this->linkLabel1->Text = L"Chưa có tài khoản\? đăng kí";
			this->linkLabel1->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &Login::registation);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::Red;
			this->label1->Location = System::Drawing::Point(112, 165);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(143, 13);
			this->label1->TabIndex = 6;
			this->label1->Text = L"Sai tài khoản hoặc mật khẩu";
			this->label1->Visible = false;
			// 
			// Login
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 261);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->linkLabel1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->userbox);
			this->Controls->Add(this->passbox);
			this->Controls->Add(this->tktext);
			this->Controls->Add(this->pstext);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"Login";
			this->Text = L"Đăng nhập";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void registation(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e) {
	Form^ reg = gcnew Client::RegForm(socket);
	this->Hide();
	reg->ShowDialog();
	this->Show();
	}
private: System::Void LoginClick(System::Object^ sender, System::EventArgs^ e) {
	this->button1->Enabled = false;
	std::string user, pass;
	this->label1->Visible = false;
	Sleep(10);
	MarshalString(this->userbox->Text, user);
	MarshalString(this->passbox->Text, pass);
	senddata(socket, Log(user, pass).c_str());
	std::string respone = revicedata(socket);
	if (respone == "Accept") {
		Form^ Go = gcnew Client::Gold(socket, this->userbox->Text);
		this->Hide();
		Go->ShowDialog();
		this->Close();
	}
	else if(respone == "Taken") {
		MessageBox::Show(L"Tài khoản đã đăng nhập tại nơi khác", L"Lỗi xác thực");
	}
	else {
		this->label1->Visible = true;
	}
	this->button1->Enabled = true;
}
private: System::Void userbox_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	if (this->userbox->Text->ToString()->Length >= 8 && this->passbox->Text->ToString()->Length >= 8) {
		this->button1->Enabled = true;
	}
	else  this->button1->Enabled = false;
}
private: System::Void passbox_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	if (this->userbox->Text->ToString()->Length >= 8 && this->passbox->Text->ToString()->Length >= 8) {
		this->button1->Enabled = true;
	}
	else  this->button1->Enabled = false;
}
};
}