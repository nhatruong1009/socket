#include "clientSocket.h"

namespace Client {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for RegForm
	/// </summary>
	public ref class RegForm : public System::Windows::Forms::Form
	{
	public:
		int socket;
		RegForm(int sock)
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
		~RegForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private:
		System::Windows::Forms::Button^ button1;
		System::Windows::Forms::PictureBox^ pictureBox1;
		System::Windows::Forms::TextBox^ userbox;
		System::Windows::Forms::TextBox^ passbox;
		System::Windows::Forms::TextBox^ passbox1;
		System::Windows::Forms::Label^ tktext;
		System::Windows::Forms::Label^ pstext;
		System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::CheckBox^ checkBoxUser;
	private: System::Windows::Forms::CheckBox^ checkBoxPass;
	private: System::Windows::Forms::CheckBox^ checkBoxRepass;
	private: System::Windows::Forms::Label^ label2;



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
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(RegForm::typeid));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->userbox = (gcnew System::Windows::Forms::TextBox());
			this->passbox = (gcnew System::Windows::Forms::TextBox());
			this->passbox1 = (gcnew System::Windows::Forms::TextBox());
			this->tktext = (gcnew System::Windows::Forms::Label());
			this->pstext = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->checkBoxUser = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxPass = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxRepass = (gcnew System::Windows::Forms::CheckBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(105, 262);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 11;
			this->button1->Text = L"Đăng kí";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &RegForm::button1_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(105, 12);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(75, 45);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 9;
			this->pictureBox1->TabStop = false;
			// 
			// userbox
			// 
			this->userbox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->userbox->Location = System::Drawing::Point(29, 88);
			this->userbox->Name = L"userbox";
			this->userbox->Size = System::Drawing::Size(233, 22);
			this->userbox->TabIndex = 5;
			this->userbox->TextChanged += gcnew System::EventHandler(this, &RegForm::userbox_TextChanged);
			// 
			// passbox
			// 
			this->passbox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->passbox->Location = System::Drawing::Point(29, 138);
			this->passbox->Name = L"passbox";
			this->passbox->PasswordChar = '*';
			this->passbox->Size = System::Drawing::Size(233, 22);
			this->passbox->TabIndex = 6;
			this->passbox->TextChanged += gcnew System::EventHandler(this, &RegForm::passbox_TextChanged);
			// 
			// passbox1
			// 
			this->passbox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->passbox1->Location = System::Drawing::Point(30, 196);
			this->passbox1->Name = L"passbox1";
			this->passbox1->PasswordChar = '*';
			this->passbox1->Size = System::Drawing::Size(233, 22);
			this->passbox1->TabIndex = 10;
			this->passbox1->TextChanged += gcnew System::EventHandler(this, &RegForm::textBox1_TextChanged);
			// 
			// tktext
			// 
			this->tktext->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->tktext->Location = System::Drawing::Point(26, 69);
			this->tktext->Name = L"tktext";
			this->tktext->Size = System::Drawing::Size(236, 25);
			this->tktext->TabIndex = 7;
			this->tktext->Text = L"Tài khoản:";
			// 
			// pstext
			// 
			this->pstext->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->pstext->Location = System::Drawing::Point(26, 117);
			this->pstext->Name = L"pstext";
			this->pstext->Size = System::Drawing::Size(236, 25);
			this->pstext->TabIndex = 8;
			this->pstext->Text = L"Mật khẩu:";
			// 
			// label1
			// 
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(26, 168);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(236, 25);
			this->label1->TabIndex = 12;
			this->label1->Text = L"Nhập lại mật khẩu:";
			// 
			// checkBoxUser
			// 
			this->checkBoxUser->AccessibleRole = System::Windows::Forms::AccessibleRole::None;
			this->checkBoxUser->AutoCheck = false;
			this->checkBoxUser->AutoSize = true;
			this->checkBoxUser->ForeColor = System::Drawing::SystemColors::Highlight;
			this->checkBoxUser->Location = System::Drawing::Point(269, 92);
			this->checkBoxUser->Name = L"checkBoxUser";
			this->checkBoxUser->Size = System::Drawing::Size(15, 14);
			this->checkBoxUser->TabIndex = 2;
			this->checkBoxUser->TabStop = false;
			this->checkBoxUser->UseCompatibleTextRendering = true;
			this->checkBoxUser->UseVisualStyleBackColor = true;
			// 
			// checkBoxPass
			// 
			this->checkBoxPass->AccessibleRole = System::Windows::Forms::AccessibleRole::None;
			this->checkBoxPass->AutoCheck = false;
			this->checkBoxPass->AutoSize = true;
			this->checkBoxPass->ForeColor = System::Drawing::SystemColors::Highlight;
			this->checkBoxPass->Location = System::Drawing::Point(269, 142);
			this->checkBoxPass->Name = L"checkBoxPass";
			this->checkBoxPass->Size = System::Drawing::Size(15, 14);
			this->checkBoxPass->TabIndex = 1;
			this->checkBoxPass->TabStop = false;
			this->checkBoxPass->UseVisualStyleBackColor = true;
			// 
			// checkBoxRepass
			// 
			this->checkBoxRepass->AccessibleRole = System::Windows::Forms::AccessibleRole::None;
			this->checkBoxRepass->AutoCheck = false;
			this->checkBoxRepass->AutoSize = true;
			this->checkBoxRepass->CausesValidation = false;
			this->checkBoxRepass->Cursor = System::Windows::Forms::Cursors::Default;
			this->checkBoxRepass->ForeColor = System::Drawing::SystemColors::MenuHighlight;
			this->checkBoxRepass->Location = System::Drawing::Point(269, 201);
			this->checkBoxRepass->Name = L"checkBoxRepass";
			this->checkBoxRepass->Size = System::Drawing::Size(15, 14);
			this->checkBoxRepass->TabIndex = 0;
			this->checkBoxRepass->TabStop = false;
			this->checkBoxRepass->UseVisualStyleBackColor = false;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ForeColor = System::Drawing::Color::Red;
			this->label2->Location = System::Drawing::Point(159, 231);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(104, 13);
			this->label2->TabIndex = 13;
			this->label2->Text = L"Tài Khoản đã tồn tại";
			this->label2->Visible = false;
			// 
			// RegForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(308, 312);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->checkBoxRepass);
			this->Controls->Add(this->checkBoxPass);
			this->Controls->Add(this->checkBoxUser);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->userbox);
			this->Controls->Add(this->passbox);
			this->Controls->Add(this->passbox1);
			this->Controls->Add(this->tktext);
			this->Controls->Add(this->pstext);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"RegForm";
			this->Text = L"Đăng kí";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void userbox_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	if (this->userbox->Text->ToString()->Length < 8) {
		this->checkBoxUser->Checked = false;
	}
	else this->checkBoxUser->Checked = true;
}
private: System::Void passbox_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	if (this->passbox->Text->ToString()->Length < 8) {
		this->checkBoxPass->Checked = false;
	}
	else this->checkBoxPass->Checked = true;
}
private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	if (this->passbox->Text->ToString() != this->passbox1->Text->ToString()) {
		this->checkBoxRepass->Checked = false;
	}
	else this->checkBoxRepass->Checked = true;
}
private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
	this->button1->Enabled = false;
	if (this->checkBoxRepass->Checked == true && this->checkBoxPass->Checked == true && this->checkBoxUser->Checked == true) {
		std::string user, password;
		this->label2->Visible = false;
		MarshalString(this->userbox->Text->ToString(), user);
		MarshalString(this->passbox->Text->ToString(), password);
		senddata(socket, Register(user, password).c_str());
		Sleep(40);
		std::string respone = revicedata(socket);
		if (respone == "Accept") {
			this->Close();
		}
		else {
			this->label2->Visible = true;
		}
	}
	this->button1->Enabled = true;
}
};
}
