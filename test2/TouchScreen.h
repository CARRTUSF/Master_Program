#pragma once

//double VAR_DX[7];

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace test2 {

	/// <summary>
	/// Summary for TouchScreen
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class TouchScreen : public System::Windows::Forms::Form
	{
	public:
		TouchScreen(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~TouchScreen()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 

	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::Button^  button3;

	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::CheckBox^  checkBox6;
	private: System::Windows::Forms::CheckBox^  checkBox5;
	private: System::Windows::Forms::CheckBox^  checkBox4;
	private: System::Windows::Forms::CheckBox^  checkBox3;
	private: System::Windows::Forms::CheckBox^  checkBox2;
	private: System::Windows::Forms::CheckBox^  checkBox14;
	private: System::Windows::Forms::CheckBox^  checkBox13;
	private: System::Windows::Forms::CheckBox^  checkBox12;
	private: System::Windows::Forms::CheckBox^  checkBox11;
	private: System::Windows::Forms::CheckBox^  checkBox10;
	private: System::Windows::Forms::CheckBox^  checkBox9;
	private: System::Windows::Forms::CheckBox^  checkBox8;
	private: System::Windows::Forms::CheckBox^  checkBox7;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(TouchScreen::typeid));
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox14 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox13 = (gcnew System::Windows::Forms::CheckBox());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox12 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox11 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox10 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox9 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox8 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox7 = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox6 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox5 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox4 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox3 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox2 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->groupBox3->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->checkBox14);
			this->groupBox3->Controls->Add(this->checkBox13);
			this->groupBox3->Controls->Add(this->button3);
			this->groupBox3->Location = System::Drawing::Point(237, 232);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(289, 111);
			this->groupBox3->TabIndex = 9;
			this->groupBox3->TabStop = false;
			// 
			// checkBox14
			// 
			this->checkBox14->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox14->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"checkBox14.BackgroundImage")));
			this->checkBox14->Location = System::Drawing::Point(201, 22);
			this->checkBox14->Name = L"checkBox14";
			this->checkBox14->Size = System::Drawing::Size(68, 69);
			this->checkBox14->TabIndex = 1;
			this->checkBox14->UseVisualStyleBackColor = true;
			this->checkBox14->CheckedChanged += gcnew System::EventHandler(this, &TouchScreen::checkBox14_CheckedChanged);
			// 
			// checkBox13
			// 
			this->checkBox13->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox13->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"checkBox13.BackgroundImage")));
			this->checkBox13->Location = System::Drawing::Point(111, 22);
			this->checkBox13->Name = L"checkBox13";
			this->checkBox13->Size = System::Drawing::Size(68, 69);
			this->checkBox13->TabIndex = 1;
			this->checkBox13->UseVisualStyleBackColor = true;
			this->checkBox13->CheckedChanged += gcnew System::EventHandler(this, &TouchScreen::checkBox13_CheckedChanged);
			// 
			// button3
			// 
			this->button3->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"button3.BackgroundImage")));
			this->button3->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->button3->Location = System::Drawing::Point(22, 22);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(68, 69);
			this->button3->TabIndex = 0;
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &TouchScreen::button3_Click);
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->checkBox12);
			this->groupBox2->Controls->Add(this->checkBox11);
			this->groupBox2->Controls->Add(this->checkBox10);
			this->groupBox2->Controls->Add(this->checkBox9);
			this->groupBox2->Controls->Add(this->checkBox8);
			this->groupBox2->Controls->Add(this->checkBox7);
			this->groupBox2->Location = System::Drawing::Point(237, 13);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(289, 213);
			this->groupBox2->TabIndex = 8;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Gripper Orientation";
			// 
			// checkBox12
			// 
			this->checkBox12->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox12->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"checkBox12.BackgroundImage")));
			this->checkBox12->Location = System::Drawing::Point(201, 127);
			this->checkBox12->Name = L"checkBox12";
			this->checkBox12->Size = System::Drawing::Size(68, 69);
			this->checkBox12->TabIndex = 1;
			this->checkBox12->UseVisualStyleBackColor = true;
			this->checkBox12->CheckedChanged += gcnew System::EventHandler(this, &TouchScreen::checkBox12_CheckedChanged);
			// 
			// checkBox11
			// 
			this->checkBox11->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox11->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"checkBox11.BackgroundImage")));
			this->checkBox11->Location = System::Drawing::Point(111, 127);
			this->checkBox11->Name = L"checkBox11";
			this->checkBox11->Size = System::Drawing::Size(68, 69);
			this->checkBox11->TabIndex = 1;
			this->checkBox11->UseVisualStyleBackColor = true;
			this->checkBox11->CheckedChanged += gcnew System::EventHandler(this, &TouchScreen::checkBox11_CheckedChanged);
			// 
			// checkBox10
			// 
			this->checkBox10->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox10->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"checkBox10.BackgroundImage")));
			this->checkBox10->Location = System::Drawing::Point(22, 127);
			this->checkBox10->Name = L"checkBox10";
			this->checkBox10->Size = System::Drawing::Size(68, 69);
			this->checkBox10->TabIndex = 1;
			this->checkBox10->UseVisualStyleBackColor = true;
			this->checkBox10->CheckedChanged += gcnew System::EventHandler(this, &TouchScreen::checkBox10_CheckedChanged);
			// 
			// checkBox9
			// 
			this->checkBox9->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox9->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"checkBox9.BackgroundImage")));
			this->checkBox9->Location = System::Drawing::Point(201, 19);
			this->checkBox9->Name = L"checkBox9";
			this->checkBox9->Size = System::Drawing::Size(68, 69);
			this->checkBox9->TabIndex = 1;
			this->checkBox9->UseVisualStyleBackColor = true;
			this->checkBox9->CheckedChanged += gcnew System::EventHandler(this, &TouchScreen::checkBox9_CheckedChanged);
			// 
			// checkBox8
			// 
			this->checkBox8->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox8->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"checkBox8.BackgroundImage")));
			this->checkBox8->Location = System::Drawing::Point(111, 19);
			this->checkBox8->Name = L"checkBox8";
			this->checkBox8->Size = System::Drawing::Size(68, 69);
			this->checkBox8->TabIndex = 1;
			this->checkBox8->UseVisualStyleBackColor = true;
			this->checkBox8->CheckedChanged += gcnew System::EventHandler(this, &TouchScreen::checkBox8_CheckedChanged);
			// 
			// checkBox7
			// 
			this->checkBox7->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox7->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"checkBox7.BackgroundImage")));
			this->checkBox7->Location = System::Drawing::Point(22, 19);
			this->checkBox7->Name = L"checkBox7";
			this->checkBox7->Size = System::Drawing::Size(68, 69);
			this->checkBox7->TabIndex = 1;
			this->checkBox7->UseVisualStyleBackColor = true;
			this->checkBox7->CheckedChanged += gcnew System::EventHandler(this, &TouchScreen::checkBox7_CheckedChanged);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->checkBox6);
			this->groupBox1->Controls->Add(this->checkBox5);
			this->groupBox1->Controls->Add(this->checkBox4);
			this->groupBox1->Controls->Add(this->checkBox3);
			this->groupBox1->Controls->Add(this->checkBox2);
			this->groupBox1->Controls->Add(this->checkBox1);
			this->groupBox1->Location = System::Drawing::Point(12, 13);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(206, 330);
			this->groupBox1->TabIndex = 7;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Gripper  Possition";
			// 
			// checkBox6
			// 
			this->checkBox6->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox6->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"checkBox6.BackgroundImage")));
			this->checkBox6->Location = System::Drawing::Point(114, 241);
			this->checkBox6->Name = L"checkBox6";
			this->checkBox6->Size = System::Drawing::Size(68, 69);
			this->checkBox6->TabIndex = 1;
			this->checkBox6->UseVisualStyleBackColor = true;
			this->checkBox6->CheckedChanged += gcnew System::EventHandler(this, &TouchScreen::checkBox6_CheckedChanged);
			// 
			// checkBox5
			// 
			this->checkBox5->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox5->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"checkBox5.BackgroundImage")));
			this->checkBox5->Location = System::Drawing::Point(25, 241);
			this->checkBox5->Name = L"checkBox5";
			this->checkBox5->Size = System::Drawing::Size(68, 69);
			this->checkBox5->TabIndex = 1;
			this->checkBox5->UseVisualStyleBackColor = true;
			this->checkBox5->CheckedChanged += gcnew System::EventHandler(this, &TouchScreen::checkBox5_CheckedChanged);
			// 
			// checkBox4
			// 
			this->checkBox4->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox4->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"checkBox4.BackgroundImage")));
			this->checkBox4->Location = System::Drawing::Point(114, 127);
			this->checkBox4->Name = L"checkBox4";
			this->checkBox4->Size = System::Drawing::Size(68, 69);
			this->checkBox4->TabIndex = 1;
			this->checkBox4->UseVisualStyleBackColor = true;
			this->checkBox4->CheckedChanged += gcnew System::EventHandler(this, &TouchScreen::checkBox4_CheckedChanged);
			// 
			// checkBox3
			// 
			this->checkBox3->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox3->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"checkBox3.BackgroundImage")));
			this->checkBox3->Location = System::Drawing::Point(25, 127);
			this->checkBox3->Name = L"checkBox3";
			this->checkBox3->Size = System::Drawing::Size(68, 69);
			this->checkBox3->TabIndex = 1;
			this->checkBox3->UseVisualStyleBackColor = true;
			this->checkBox3->CheckedChanged += gcnew System::EventHandler(this, &TouchScreen::checkBox3_CheckedChanged);
			// 
			// checkBox2
			// 
			this->checkBox2->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox2->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"checkBox2.BackgroundImage")));
			this->checkBox2->Location = System::Drawing::Point(114, 19);
			this->checkBox2->Name = L"checkBox2";
			this->checkBox2->Size = System::Drawing::Size(68, 69);
			this->checkBox2->TabIndex = 1;
			this->checkBox2->UseVisualStyleBackColor = true;
			this->checkBox2->CheckedChanged += gcnew System::EventHandler(this, &TouchScreen::checkBox2_CheckedChanged);
			// 
			// checkBox1
			// 
			this->checkBox1->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"checkBox1.BackgroundImage")));
			this->checkBox1->Location = System::Drawing::Point(25, 19);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(68, 69);
			this->checkBox1->TabIndex = 1;
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &TouchScreen::checkBox1_CheckedChanged);
			// 
			// button4
			// 
			this->button4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button4->Location = System::Drawing::Point(351, 357);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(75, 30);
			this->button4->TabIndex = 5;
			this->button4->Text = L"Run";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &TouchScreen::button4_Click);
			// 
			// button2
			// 
			this->button2->Enabled = false;
			this->button2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button2->Location = System::Drawing::Point(12, 357);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(78, 30);
			this->button2->TabIndex = 6;
			this->button2->Text = L"Back";
			this->button2->UseVisualStyleBackColor = true;
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button1->Location = System::Drawing::Point(451, 357);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 30);
			this->button1->TabIndex = 5;
			this->button1->Text = L"Exit";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &TouchScreen::button1_Click);
			// 
			// TouchScreen
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(539, 401);
			this->ControlBox = false;
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->MaximizeBox = false;
			this->Name = L"TouchScreen";
			this->Text = L"WMRA Screen";
			this->Load += gcnew System::EventHandler(this, &TouchScreen::TouchScreen_Load);
			this->groupBox3->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				 switch(checkBox1->CheckState) {
		 case CheckState::Checked:
			 // Code for checked state.
			 this->checkBox1->FlatStyle = FlatStyle::Flat;
			 this->checkBox1->FlatAppearance->BorderSize=2;
			 this->checkBox1->FlatAppearance->BorderColor=Color::Red;
			 this->checkBox5->Enabled=false;
			 VAR_DX[2]=1;  
			 empa[0]=1;
            break;
         case CheckState::Unchecked:
			 // Code for unchecked state.
			 this->checkBox1->FlatStyle = FlatStyle::Standard;				 
			 this->checkBox5->Enabled=true;
			 VAR_DX[2]=0; 
			 empa[0]=0;
            break;
         case CheckState::Indeterminate:
			 // Code for indeterminate state.
			 VAR_DX[2]=0;
			 this->checkBox1->FlatStyle = FlatStyle::Standard;	
			break;
      }
			 }
private: System::Void checkBox2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 switch(checkBox2->CheckState) {
         case CheckState::Checked:
			 // Code for checked state.
			 this->checkBox2->FlatStyle = FlatStyle::Flat;
			 this->checkBox2->FlatAppearance->BorderSize=2;
			 this->checkBox2->FlatAppearance->BorderColor=Color::Red;
			 this->checkBox6->Enabled=false;
			 VAR_DX[0]=1;     
			 empa[1]=1;
            break;
         case CheckState::Unchecked:
			 // Code for unchecked state.
			 this->checkBox2->FlatStyle = FlatStyle::Standard;				 
			 this->checkBox6->Enabled=true;
			 VAR_DX[0]=0;
			 empa[1]=0;
            break;
         case CheckState::Indeterminate:
			 // Code for indeterminate state.
			 VAR_DX[0]=0;			 
			 this->checkBox2->FlatStyle = FlatStyle::Standard;	            
            break;
      }
		 }
private: System::Void checkBox3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 switch(checkBox3->CheckState) {
         case CheckState::Checked:
			 // Code for checked state.
			 this->checkBox3->FlatStyle = FlatStyle::Flat;
			 this->checkBox3->FlatAppearance->BorderSize=2;
			 this->checkBox3->FlatAppearance->BorderColor=Color::Red;
			 this->checkBox4->Enabled=false;
			 VAR_DX[1]=1;
			 empa[2]=1;
            break;
         case CheckState::Unchecked:
			 // Code for unchecked state.
			 this->checkBox3->FlatStyle = FlatStyle::Standard;				 
			 this->checkBox4->Enabled=true;
			 VAR_DX[1]=0;
			 empa[2]=0;
            break;
         case CheckState::Indeterminate:
			 // Code for indeterminate state.
			 VAR_DX[1]=0;
			 this->checkBox3->FlatStyle = FlatStyle::Standard;	  
			break;
      }
		 }
private: System::Void checkBox4_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 switch(checkBox4->CheckState) {
         case CheckState::Checked:
			 // Code for checked state.
			 this->checkBox4->FlatStyle = FlatStyle::Flat;
			 this->checkBox4->FlatAppearance->BorderSize=2;
			 this->checkBox4->FlatAppearance->BorderColor=Color::Red;
			 this->checkBox3->Enabled=false;
			 VAR_DX[1]=-1;
			 empa[3]=1;
            break;
         case CheckState::Unchecked:
			 // Code for unchecked state.
			 this->checkBox4->FlatStyle = FlatStyle::Standard;				 
			 this->checkBox3->Enabled=true;
			 VAR_DX[1]=0;
			 empa[3]=0;
            break;
         case CheckState::Indeterminate:
			 // Code for indeterminate state.
			 VAR_DX[1]=0;
			 this->checkBox4->FlatStyle = FlatStyle::Standard;	  
			break;
      }
		 }
private: System::Void checkBox5_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 switch(checkBox5->CheckState) {
		 case CheckState::Checked:
			 // Code for checked state.
			 this->checkBox5->FlatStyle = FlatStyle::Flat;
			 this->checkBox5->FlatAppearance->BorderSize=2;
			 this->checkBox5->FlatAppearance->BorderColor=Color::Red;
			 this->checkBox1->Enabled=false;
			 VAR_DX[2]=-1;    
			 empa[4]=1;
            break;
         case CheckState::Unchecked:
			 // Code for unchecked state.
			 this->checkBox5->FlatStyle = FlatStyle::Standard;				 
			 this->checkBox1->Enabled=true;
			 VAR_DX[2]=0;
			 empa[4]=0;
            break;
         case CheckState::Indeterminate:
			 // Code for indeterminate state.
			 VAR_DX[2]=0;
			 this->checkBox5->FlatStyle = FlatStyle::Standard;		
            break;
      }
		 }
private: System::Void checkBox6_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 switch(checkBox6->CheckState) {
         case CheckState::Checked:
			 // Code for checked state.
			 this->checkBox6->FlatStyle = FlatStyle::Flat;
			 this->checkBox6->FlatAppearance->BorderSize=2;
			 this->checkBox6->FlatAppearance->BorderColor=Color::Red;
			 this->checkBox2->Enabled=false;
			 VAR_DX[0]=-1; 
			 empa[5]=1;
            break;
         case CheckState::Unchecked:
			 // Code for unchecked state.
			 this->checkBox6->FlatStyle = FlatStyle::Standard;				 
			 this->checkBox2->Enabled=true;
			 VAR_DX[0]=0;
			 empa[5]=0;
            break;
         case CheckState::Indeterminate:
			 // Code for indeterminate state.
			 VAR_DX[0]=0;			 
			 this->checkBox6->FlatStyle = FlatStyle::Standard;	            
            break;
      }
		 }
private: System::Void checkBox7_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			  switch(checkBox7->CheckState) {
         case CheckState::Checked:
			 // Code for checked state.
			 this->checkBox7->FlatStyle = FlatStyle::Flat;
			 this->checkBox7->FlatAppearance->BorderSize=2;
			 this->checkBox7->FlatAppearance->BorderColor=Color::Red;
			 this->checkBox9->Enabled=false;
			 VAR_DX[5]=0.003; 
			 empa[6]=1;
            break;
         case CheckState::Unchecked:
			 // Code for unchecked state.
			 this->checkBox7->FlatStyle = FlatStyle::Standard;				 
			 this->checkBox9->Enabled=true;
			 VAR_DX[5]=0;
			 empa[6]=0;
            break;
         case CheckState::Indeterminate:
			 // Code for indeterminate state.
			 VAR_DX[5]=0;			 
			 this->checkBox7->FlatStyle = FlatStyle::Standard;	            
            break;
      }
		 }
private: System::Void checkBox8_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 switch(checkBox8->CheckState) {
         case CheckState::Checked:
			 // Code for checked state.
			 this->checkBox8->FlatStyle = FlatStyle::Flat;
			 this->checkBox8->FlatAppearance->BorderSize=2;
			 this->checkBox8->FlatAppearance->BorderColor=Color::Red;
			 this->checkBox11->Enabled=false;
			 VAR_DX[4]=0.003;       
			 empa[7]=1;
            break;
         case CheckState::Unchecked:
			 // Code for unchecked state.
			 this->checkBox8->FlatStyle = FlatStyle::Standard;				 
			 this->checkBox11->Enabled=true;
			 VAR_DX[4]=0;
			 empa[7]=0;
            break;
         case CheckState::Indeterminate:
			 // Code for indeterminate state.
			 VAR_DX[4]=0;			 
			 this->checkBox8->FlatStyle = FlatStyle::Standard;	            
            break;
      }
		 }
private: System::Void checkBox9_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 switch(checkBox9->CheckState) {
         case CheckState::Checked:
			 // Code for checked state.
			 this->checkBox9->FlatStyle = FlatStyle::Flat;
			 this->checkBox9->FlatAppearance->BorderSize=2;
			 this->checkBox9->FlatAppearance->BorderColor=Color::Red;
			 this->checkBox7->Enabled=false;
			 VAR_DX[5]=-0.003;     
			 empa[8]=1;
            break;
         case CheckState::Unchecked:
			 // Code for unchecked state.
			 this->checkBox9->FlatStyle = FlatStyle::Standard;				 
			 this->checkBox7->Enabled=true;
			 VAR_DX[5]=0;
			 empa[8]=0;
            break;
         case CheckState::Indeterminate:
			 // Code for indeterminate state.
			 VAR_DX[5]=0;			 
			 this->checkBox9->FlatStyle = FlatStyle::Standard;	            
            break;
      }
		 }
private: System::Void checkBox10_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 switch(checkBox10->CheckState) {
         case CheckState::Checked:
			 // Code for checked state.
			 this->checkBox10->FlatStyle = FlatStyle::Flat;
			 this->checkBox10->FlatAppearance->BorderSize=2;
			 this->checkBox10->FlatAppearance->BorderColor=Color::Red;
			 this->checkBox12->Enabled=false;
			 VAR_DX[3]=-0.003;      
			 empa[9]=1;
            break;
         case CheckState::Unchecked:
			 // Code for unchecked state.
			 this->checkBox10->FlatStyle = FlatStyle::Standard;				 
			 this->checkBox12->Enabled=true;
			 VAR_DX[3]=0;
			 empa[9]=0;
            break;
         case CheckState::Indeterminate:
			 // Code for indeterminate state.
			 VAR_DX[3]=0;			 
			 this->checkBox10->FlatStyle = FlatStyle::Standard;	            
            break;
      }
		 }
private: System::Void checkBox11_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 switch(checkBox11->CheckState) {
         case CheckState::Checked:
			 // Code for checked state.
			 this->checkBox11->FlatStyle = FlatStyle::Flat;
			 this->checkBox11->FlatAppearance->BorderSize=2;
			 this->checkBox11->FlatAppearance->BorderColor=Color::Red;
			 this->checkBox8->Enabled=false;
			 VAR_DX[4]=-0.003;     
			 empa[10]=1;
            break;
         case CheckState::Unchecked:
			 // Code for unchecked state.
			 this->checkBox11->FlatStyle = FlatStyle::Standard;				 
			 this->checkBox8->Enabled=true;
			 VAR_DX[4]=0;
			 empa[10]=0;
            break;
         case CheckState::Indeterminate:
			 // Code for indeterminate state.
			 VAR_DX[4]=0;			 
			 this->checkBox11->FlatStyle = FlatStyle::Standard;	            
            break;
      }
		 }
private: System::Void checkBox12_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 switch(checkBox12->CheckState) {
         case CheckState::Checked:
			 // Code for checked state.
			 this->checkBox12->FlatStyle = FlatStyle::Flat;
			 this->checkBox12->FlatAppearance->BorderSize=2;
			 this->checkBox12->FlatAppearance->BorderColor=Color::Red;
			 this->checkBox10->Enabled=false;
			 VAR_DX[3]=0.003;            
			 empa[11]=1;
            break;
         case CheckState::Unchecked:
			 // Code for unchecked state.
			 this->checkBox12->FlatStyle = FlatStyle::Standard;				 
			 this->checkBox10->Enabled=true;
			 VAR_DX[3]=0;
			 empa[11]=0;
            break;
         case CheckState::Indeterminate:
			 // Code for indeterminate state.
			 VAR_DX[3]=0;			 
			 this->checkBox12->FlatStyle = FlatStyle::Standard;	            
            break;
      }
		 }
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
			 VAR_DX[0]=0;
			 VAR_DX[1]=0;
			 VAR_DX[2]=0;
			 VAR_DX[3]=0;
			 VAR_DX[4]=0;
			 VAR_DX[5]=0;
			 VAR_DX[6]=0;
			 
			 this->checkBox1->Enabled=true;
			 this->checkBox2->Enabled=true;
			 this->checkBox3->Enabled=true;
			 this->checkBox4->Enabled=true;
			 this->checkBox5->Enabled=true;
			 this->checkBox6->Enabled=true;
			 this->checkBox7->Enabled=true;
			 this->checkBox8->Enabled=true;
			 this->checkBox9->Enabled=true;
			 this->checkBox10->Enabled=true;
			 this->checkBox11->Enabled=true;
			 this->checkBox12->Enabled=true;
			 this->checkBox13->Enabled=true;
			 this->checkBox14->Enabled=true;

			 this->checkBox1->Checked = false;
			 this->checkBox2->Checked = false;
			 this->checkBox3->Checked = false;
			 this->checkBox4->Checked = false;
			 this->checkBox5->Checked = false;
			 this->checkBox6->Checked = false;
			 this->checkBox7->Checked = false;
			 this->checkBox8->Checked = false;
			 this->checkBox9->Checked = false;
			 this->checkBox10->Checked = false;
			 this->checkBox11->Checked = false;
			 this->checkBox12->Checked = false;
			 this->checkBox13->Checked = false;
			 this->checkBox14->Checked = false;
		 }
private: System::Void checkBox13_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			  switch(checkBox13->CheckState) {
         case CheckState::Checked:
			 // Code for checked state.
			 this->checkBox13->FlatStyle = FlatStyle::Flat;
			 this->checkBox13->FlatAppearance->BorderSize=2;
			 this->checkBox13->FlatAppearance->BorderColor=Color::Red;
			 this->checkBox14->Enabled=false;
			 VAR_DX[6]=1;   
			 empa[12]=1;  
            break;
         case CheckState::Unchecked:
			 // Code for unchecked state.
			 this->checkBox13->FlatStyle = FlatStyle::Standard;				 
			 this->checkBox14->Enabled=true;
			 VAR_DX[6]=0;
			 empa[12]=0; 
            break;
         case CheckState::Indeterminate:
			 // Code for indeterminate state.
			 VAR_DX[6]=0;			 
			 this->checkBox12->FlatStyle = FlatStyle::Standard;	            
            break;
      }
		 }
private: System::Void checkBox14_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			   switch(checkBox14->CheckState) {
         case CheckState::Checked:
			 // Code for checked state.
			 this->checkBox14->FlatStyle = FlatStyle::Flat;
			 this->checkBox14->FlatAppearance->BorderSize=2;
			 this->checkBox14->FlatAppearance->BorderColor=Color::Red;
			 this->checkBox13->Enabled=false;
			 VAR_DX[6]=-1;  
			 empa[13]=1;
            break;
         case CheckState::Unchecked:
			 // Code for unchecked state.
			 this->checkBox14->FlatStyle = FlatStyle::Standard;				 
			 this->checkBox13->Enabled=true;
			 VAR_DX[6]=0;
			 empa[13]=0;
            break;
         case CheckState::Indeterminate:
			 // Code for indeterminate state.
			 VAR_DX[6]=0;			 
			 this->checkBox14->FlatStyle = FlatStyle::Standard;	            
            break;
      }
		 }
public: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			varscreenopn = 0; 
			TouchScreen::Close();
		 }
public: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
			varscreenopn = 1; 
			TouchScreen::Hide();
		 }
private: System::Void TouchScreen_Load(System::Object^  sender, System::EventArgs^  e) {
			 TouchScreen::CenterToScreen(); 
		//	 cout<<"siiiiii";
			 varscreenopn = 1;
			 if (empanadis!=0){
				 if(empa[0]==1){checkBox1->Checked=true;}
				 if(empa[1]==1){checkBox2->Checked=true;}
				 if(empa[2]==1){checkBox3->Checked=true;}
				 if(empa[3]==1){checkBox4->Checked=true;}
				 if(empa[4]==1){checkBox5->Checked=true;}
				 if(empa[5]==1){checkBox6->Checked=true;}
				 if(empa[6]==1){checkBox7->Checked=true;}
				 if(empa[7]==1){checkBox8->Checked=true;}
				 if(empa[8]==1){checkBox9->Checked=true;}
				 if(empa[9]==1){checkBox10->Checked=true;}
				 if(empa[10]==1){checkBox11->Checked=true;}
				 if(empa[11]==1){checkBox12->Checked=true;}
				 if(empa[12]==1){checkBox13->Checked=true;}
				 if(empa[13]==1){checkBox14->Checked=true;}
				// cout<<"Sale o no si: "<<varscreenopn<<"\n\n";
			 }
		 }
};
}
