#pragma once
#include <sstream>
#include <string>
#include <fstream>
#include"clientSocket.h"
#include<codecvt>
#include <conio.h>
namespace Client {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Gold
	/// </summary>
	public ref class Gold : public System::Windows::Forms::Form
	{
	public:
		int socket;
		std::wstring* gold, * province;
		int nGold = 0, nProvince = 0;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::DateTimePicker^ dateTimePicker1;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::LinkLabel^ linkLabel1;

	public:
	private: System::Windows::Forms::ListView^ listView1;
	public:

	public:











	public:

	public:
		Gold(int sock,System::String^ user)
		{	
			socket = sock;
			InitializeComponent();
			start(user);
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Gold()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListBox^ FistChoseBox;
	private: System::Windows::Forms::Button^ type1;
	private: System::Windows::Forms::Button^ type2;

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
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(Gold::typeid));
			this->FistChoseBox = (gcnew System::Windows::Forms::ListBox());
			this->type1 = (gcnew System::Windows::Forms::Button());
			this->type2 = (gcnew System::Windows::Forms::Button());
			this->listView1 = (gcnew System::Windows::Forms::ListView());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->dateTimePicker1 = (gcnew System::Windows::Forms::DateTimePicker());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->linkLabel1 = (gcnew System::Windows::Forms::LinkLabel());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// FistChoseBox
			// 
			this->FistChoseBox->BackColor = System::Drawing::Color::White;
			this->FistChoseBox->FormattingEnabled = true;
			this->FistChoseBox->Location = System::Drawing::Point(12, 219);
			this->FistChoseBox->Name = L"FistChoseBox";
			this->FistChoseBox->ScrollAlwaysVisible = true;
			this->FistChoseBox->Size = System::Drawing::Size(168, 121);
			this->FistChoseBox->TabIndex = 1;
			// 
			// type1
			// 
			this->type1->Location = System::Drawing::Point(12, 190);
			this->type1->Name = L"type1";
			this->type1->Size = System::Drawing::Size(168, 23);
			this->type1->TabIndex = 2;
			this->type1->Text = L"Theo vàng";
			this->type1->UseVisualStyleBackColor = true;
			this->type1->Click += gcnew System::EventHandler(this, &Gold::type1_Click);
			// 
			// type2
			// 
			this->type2->Location = System::Drawing::Point(12, 190);
			this->type2->Name = L"type2";
			this->type2->Size = System::Drawing::Size(168, 23);
			this->type2->TabIndex = 3;
			this->type2->Text = L"Theo Tỉnh";
			this->type2->UseVisualStyleBackColor = true;
			this->type2->Visible = false;
			this->type2->Click += gcnew System::EventHandler(this, &Gold::type2_Click);
			// 
			// listView1
			// 
			this->listView1->AutoArrange = false;
			this->listView1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->listView1->FullRowSelect = true;
			this->listView1->HideSelection = false;
			this->listView1->Location = System::Drawing::Point(228, 106);
			this->listView1->MultiSelect = false;
			this->listView1->Name = L"listView1";
			this->listView1->Size = System::Drawing::Size(521, 320);
			this->listView1->TabIndex = 4;
			this->listView1->UseCompatibleStateImageBehavior = false;
			this->listView1->View = System::Windows::Forms::View::Details;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(187, 270);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(35, 23);
			this->button1->TabIndex = 5;
			this->button1->Text = L">>";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Gold::button1_Click);
			// 
			// dateTimePicker1
			// 
			this->dateTimePicker1->CustomFormat = L"dd/MM/yyyy";
			this->dateTimePicker1->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dateTimePicker1->Location = System::Drawing::Point(13, 364);
			this->dateTimePicker1->Name = L"dateTimePicker1";
			this->dateTimePicker1->Size = System::Drawing::Size(167, 20);
			this->dateTimePicker1->TabIndex = 6;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(13, 23);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(167, 161);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 7;
			this->pictureBox1->TabStop = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(222, 12);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(367, 31);
			this->label1->TabIndex = 8;
			this->label1->Text = L"Chào mừng xxx đến với Goldus";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// linkLabel1
			// 
			this->linkLabel1->AutoSize = true;
			this->linkLabel1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->linkLabel1->Location = System::Drawing::Point(225, 43);
			this->linkLabel1->Name = L"linkLabel1";
			this->linkLabel1->Size = System::Drawing::Size(74, 18);
			this->linkLabel1->TabIndex = 9;
			this->linkLabel1->TabStop = true;
			this->linkLabel1->Text = L"Đăng xuất";
			this->linkLabel1->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &Gold::linkLabel1_LinkClicked);
			// 
			// Gold
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->ClientSize = System::Drawing::Size(761, 438);
			this->Controls->Add(this->linkLabel1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->dateTimePicker1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->listView1);
			this->Controls->Add(this->type2);
			this->Controls->Add(this->type1);
			this->Controls->Add(this->FistChoseBox);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"Gold";
			this->Text = L"Gold";
			this->Load += gcnew System::EventHandler(this, &Gold::Gold_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void Gold_Load(System::Object^ sender, System::EventArgs^ e) {
		senddata(socket, "NAME");
		char k[4];
		recv(socket, k, 4, 0);
		int n = *(int*)&*k;
		char* temp = new char[n + 2];
		recv(socket, temp, n + 1, 0);
		temp[n+1] = '\0';
		std::wstring num;
		std::wfstream kk("GoldName.dat", std::ios_base::out | std::ios_base::binary);
		kk.write((wchar_t*)&*temp, n + 1);
		delete[] temp;
		kk.close();
		kk.open("GoldName.dat", std::ios_base::in);
		std::getline(kk, num);
		std::stoi(num);
		nGold = n;
		gold = new std::wstring[n];
		for (int i = 0; i < n; i++) {
			std::getline(kk, gold[i]);
			this->FistChoseBox->Items->Add(gcnew System::String(gold[i].c_str()));
		}
		kk.close();
		recv(socket, k, 4, 0);
		n = *(int*)&*k;
		temp = new char[n + 2];
		recv(socket, temp, n + 1, 0);
		temp[n + 1] = '\0';

		kk.open("Province.dat", std::ios_base::out | std::ios_base::binary);
		kk.write((wchar_t*)&*temp, n + 1);
		delete[] temp;
		kk.close();

		kk.open("Province.dat", std::ios_base::in);
		kk.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		std::getline(kk, num);
		n = std::stoi(num);
		nProvince = n;
		province = new std::wstring[n];
		
		for (int i = 0; i < n; i++) {
			std::getline(kk, province[i], L'\n');
			if (province[i] == L"") province[i] = L"Giá buôn";
		}
		kk.close();
	}
	private: void ShowTable(std::wstring ss) {
		this->listView1->Items->Clear();
		if (type1->Visible == false) {
			std::wstringstream str(ss);
			std::wstring temp;
			while (!str.eof())
			{
				std::getline(str, temp, L'\"');
				std::getline(str, temp, L'\"');
				ListViewItem^ item = gcnew ListViewItem(gcnew String(temp.c_str()));
				std::getline(str, temp, L'\"');
				std::getline(str, temp, L'\"');
				std::getline(str, temp, L'\"');
				std::getline(str, temp, L'\"');
				item->SubItems->Add(gcnew String(temp.c_str()));
				std::getline(str, temp, L':');
				std::getline(str, temp, L'\"');
				std::getline(str, temp, L'\"');
				item->SubItems->Add(gcnew String(temp.c_str()));
				std::getline(str, temp, L',');
				this->listView1->Items->Add(item);
			}
		}
		else {
			std::wstringstream str(ss);
			std::wstring temp;
			int n = 0;
			n = ss.find(L"buy", n);
			str.seekg(0);
			do
			{
				int m = n;
				str.seekg(n + 4);
				std::getline(str, temp, L'\"');
				std::wstring sell = L"sell_" + temp;
				ListViewItem^ item = gcnew ListViewItem(gcnew String(temp.c_str()));
				m = ss.find(L":", m);
				str.seekg(m + 1);
				std::getline(str, temp, L'.');
				item->SubItems->Add(gcnew String(temp.c_str()));
				m = ss.find(sell);
				str.seekg(m + sell.length() + 3);
				std::getline(str, temp, L'.');
				item->SubItems->Add(gcnew String(temp.c_str()));
				n = ss.find(L"buy", n + 3);
				this->listView1->Items->Add(item);
			} while (n != -1);
		}
	}
	private: System::Void type2_Click(System::Object^ sender, System::EventArgs^ e) {
		this->type2->Visible = false;
		this->type1->Visible = true;
		this->FistChoseBox->Items->Clear();
		for (int i = 0; i < nGold; i++) {
			this->FistChoseBox->Items->Add(gcnew System::String(gold[i].c_str()));
		}
	}
	private: System::Void type1_Click(System::Object^ sender, System::EventArgs^ e) {
		this->type1->Visible = false;
		this->type2->Visible = true;
		this->FistChoseBox->Items->Clear();
		for (int i = 0; i < nProvince; i++) {
			this->FistChoseBox->Items->Add(gcnew System::String(province[i].c_str()));
		}
	}
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		if (
			(this->type1->Visible==true && this->FistChoseBox->SelectedIndex>=nGold )||
			(this->type2->Visible == true && this->FistChoseBox->SelectedIndex >= nProvince)
			) {

			return;
		}
		std::string resquest = "GET type:";
		resquest += this->type1->Visible ? "0" : "1";
		resquest += "/gold:" + std::to_string(this->FistChoseBox->SelectedIndex);
		resquest += "/day:";
		std::string date;
		MarshalString(this->dateTimePicker1->Text, date);
		resquest += date;
		senddata(socket, resquest.c_str());
		std::string response = revicedata(socket);
		if (response == "Fail") { this->listView1->Items->Clear(); return; }
		std::ofstream fo("handle.txt");
		fo << response;
		fo.close();
		std::wifstream fo2("handle.txt");
		fo2.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		std::wstring kkk;
		std::getline(fo2, kkk, L'\0');
		fo2.close();
		ShowTable(kkk);
	}
	private:void start(System::String^ user) {
		this->listView1->Columns->Add(L"Loại vàng", this->listView1->Size.Width / 2);
		this->listView1->Columns->Add(L"Giá mua", this->listView1->Size.Width / 4.1);
		this->listView1->Columns->Add(L"Giá bán", this->listView1->Size.Width / 4.1);
		this->label1->Text = L"Chào mừng " + user + L" đến với Goldus";
	}
private: System::Void linkLabel1_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e) {
	if (gold) delete[] gold;
	if (province) delete[] province;
	senddata(socket, "OUT");
	this->Close();
}
};
}
