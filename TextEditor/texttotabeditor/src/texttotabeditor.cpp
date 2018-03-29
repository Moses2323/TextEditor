#include <texttotabeditor.h>

#include <filetotabwidget.h>
#include "filetotab_priv.h"
#include "formistakes.h"

#include <QDebug>
#include <QPushButton>
#include <QBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QLineEdit>

#include <fstream>

namespace fttw{

TextToTabEditorShell::TextToTabEditorShell(QWidget *obj) : QWidget(obj){
    mL_ = new QVBoxLayout;

    buttonLayout_ = new QHBoxLayout;
	openFileButton_ = new QPushButton;
	openFileButton_->setText(tr("Open file..."));		// in retranslate
    inFileLab_ = new QLineEdit("");
    inFileLab_->setReadOnly(true);

    buttonLayout_->addWidget(inFileLab_, 1000);
    buttonLayout_->addWidget(openFileButton_);

    mL_->addLayout(buttonLayout_);

    connect(openFileButton_, SIGNAL(clicked()), this, SLOT(chooseFileForEditing()));			//связывает открытие диалогового окна с кнопкой "Open in-file..."
    connect(this->get_inFileLineEditor(), SIGNAL(textChanged(QString)), this, SLOT(openFileForEditing(QString)));	//связывает изменение в строке с in-file-ом с открытием программы в текстовом редакторе

    setLayout(mL_);
}

QString TextToTabEditorShell::get_inFileName() const {return inFileLab_->text();}

bool TextToTabEditorShell::checkExistenceAndCreateIfNot(const QString &filename) const{
	std::ifstream fin;
	fin.open(filename.toStdString().c_str(), std::ios::in);
	if (!fin.is_open()){
		//не можем открыть файл, т.к. его нет => создаем пустой файл.
		fin.close();

		std::ofstream fout;
		fout.open(filename.toStdString().c_str(), std::ios::out);
		if (!fout.is_open()){			//не можем создать файл, т.к., скорее всего, нет либо прав доступа, либо нет папки, в которой файл должен лежать.
			fttw::print_mistakeLite(std::string(__FUNCTION__)+"(const QString&)", __LINE__, __FILE__, "Can\'t create file with name:\n" + filename.toStdString() + "\nmay be you forgot to create folders?\n");

//			static const QString attTitle[IAtlas::Language_Count]{
//				tr("Attention"),
//				tr("Внимание")
//			};
//			static const QString attMess1[IAtlas::Language_Count]{
//				tr("Can\'t create file with name:"),
//				tr("Не могу создать файл с именем:")
//			};
//			static const QString attMess2[IAtlas::Language_Count]{
//				tr("may be you forgot to create folders or you have no permission to create file in this folder?"),
//				tr("возможно, Вы забыли создать папку или у Вас нет прав для создания файлов в этой папке?")
//			};

//			QMessageBox::warning(0, attTitle[IAtlas::instance()->language()], attMess1[IAtlas::instance()->language()]+"\n" + filename + "\n" + attMess2[IAtlas::instance()->language()]);
			return false;
		}

		fout.close();
	}
	else{
		fin.close();
	}
	return true;
}

void TextToTabEditorShell::chooseFileForEditing(){
	QString str = QFileDialog::getSaveFileName(Q_NULLPTR, tr("Choose in-file"), defaultDir_, filters_);
	if (str.size() == 0) return;
	if (checkExistenceAndCreateIfNot(str))
		inFileLab_->setText(str);
}

void TextToTabEditorShell::openFileForEditing(QString str){
	if (fileToTab_ != nullptr){
		//удалить внутренний FileToTabWidget с помощью reload_slot.
		fileToTab_->set_loadName(str.toStdString());
		fileToTab_->set_saveName(str.toStdString());
		fileToTab_->reload_slot();
	}
	else{
		fileToTab_ = new fttw::OutSideWidget(str.toStdString());
		mL_->addWidget(fileToTab_);
	}
}

void TextToTabEditorShell::openFileForEditingWithLineEditor(const QString& str){
	if (str != inFileLab_->text()){
		if (checkExistenceAndCreateIfNot(str))
			inFileLab_->setText(str);
	}
}

void TextToTabEditorShell::retranslate(){
	openFileButton_->setText(tr("Open file..."));
	if (fileToTab_ != nullptr)
		fileToTab_->retranslate();
}

} // namespace fttw
