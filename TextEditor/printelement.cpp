#include "printelement.h"

//---------- PrintElement

size_t fttw::PrintElement::nextSerialNum = 0;

const QString fttw::TablePrintElem::trigger = "#matrix";
const QString fttw::TablePrintElem::triggerName = "#matrixname";
const QString fttw::VectorsPrintElem::trigger = "#vectors";
const QString fttw::VectorsPrintElem::triggername = "#vectorsname";
const QString fttw::VectorPrintElem::trigger = "#vector";
const QString fttw::GlobalValuePrintElem::trigger = "#scalar";
const QString fttw::GlobalValuesPrintElem::trigger = fttw::GlobalValuePrintElem::trigger;
const char fttw::CommentElement::trigger = '#';

//---------- TablePrintElem

std::ostream& fttw::TablePrintElem::printElem(std::ostream &s) const{
	if (name_ != ""){
		s<<triggerName.toStdString()<<(name_).toStdString()<<std::endl;
	}

	s<<trigger.toStdString();
	size_t Nrow, Ncol;
	if (table_ == nullptr){
		std::cout<<"ERROR in "<<__FUNCTION__<<"(ostream&) const : line "<<__LINE__<<", file "<<__FILE__<<std::endl;
		std::cout<<"\ttable was not declared in TablePrintElem"<<std::endl;
		exit(-1);
	}

	Nrow = table_->rowCount();
	Ncol = table_->columnCount();

	for(size_t j=0; j<Ncol; ++j){
		s<<" "<<table_->horizontalHeaderItem(j)->text().toStdString();
	}
	s<<std::endl;

	for(size_t j=0; j<Nrow; ++j){
		for(size_t k=0; k<Ncol-1; ++k){
			s<<table_->item(j,k)->text().toStdString()<<"\t";
		}
		s<<table_->item(j,Ncol-1)->text().toStdString()<<std::endl;
	}
	return s;
}

//---------- VectorsPrintElem

std::ostream& fttw::VectorsPrintElem::printElem(std::ostream &s) const{
	if (name_ != ""){
		s<<triggername.toStdString()<<(name_).toStdString()<<std::endl;
	}

	s<<trigger.toStdString();
	size_t Nvec, Nnum;
	if (vectors_ == nullptr){
		std::cout<<"ERROR in "<<__FUNCTION__<<"(ostream&) const : line "<<__LINE__<<", file "<<__FILE__<<std::endl;
		std::cout<<"\tvectors were not declared in VectorsPrintElem"<<std::endl;
		exit(-1);
	}

	Nvec = vectors_->rowCount();
	Nnum = vectors_->columnCount();

	for(size_t j=0; j<Nvec; ++j){
		s<<" "<<vectors_->verticalHeaderItem(j)->text().toStdString();
	}
	s<<std::endl;

	for(size_t j=0; j<Nvec; ++j){
		for(size_t k=0; k<Nnum-1; ++k){
			s<<vectors_->item(j,k)->text().toStdString()<<"\t";
		}
		s<<vectors_->item(j,Nnum-1)->text().toStdString()<<std::endl;
	}
	return s;
}

//---------- VectorPrintElem

std::ostream& fttw::VectorPrintElem::printElem(std::ostream &s) const{
	if (vector_ == nullptr){
		std::cout<<"ERROR in "<<__FUNCTION__<<"(ostream&) const : line "<<__LINE__<<", file "<<__FILE__<<std::endl;
		std::cout<<"\tvector was not declared in VectorPrintElem"<<std::endl;
		exit(-1);
	}
	if (vector_->rowCount() != 1){
		std::cout<<"ERROR in "<<__FUNCTION__<<"(ostream&) const : line "<<__LINE__<<", file "<<__FILE__<<std::endl;
		std::cout<<"\tQTableWidget in VectorPrintElem has more than 1 row. Vector must have only one row."<<std::endl;
		exit(-1);
	}
	s<<trigger.toStdString()<<" "<<name_.toStdString()<<std::endl;
	int N = vector_->columnCount();
	for(int i=0; i<N-1; ++i){
		s<<vector_->item(0,i)->text().toStdString()<<"\t";
	}
	s<<vector_->item(0,N-1)->text().toStdString()<<std::endl;

	return s;
}

//---------- GlobalValuePrintElem

std::ostream& fttw::GlobalValuePrintElem::printElem(std::ostream &s) const{
	if (label_ == nullptr){
		std::cout<<"ERROR in "<<__FUNCTION__<<"(ostream&) const : line "<<__LINE__<<", file "<<__FILE__<<std::endl;
		std::cout<<"\tLabel was not declared in GlobalValuePrintElem"<<std::endl;
		exit(-1);
	}
	if (editor_ == nullptr){
		std::cout<<"ERROR in "<<__FUNCTION__<<"(ostream&) const : line "<<__LINE__<<", file "<<__FILE__<<std::endl;
		std::cout<<"\tLineEditor was not declared in GlobalValuePrintElem"<<std::endl;
		std::cout<<"Label = "<<label_->text().toStdString()<<std::endl;
		exit(-1);
	}

	s<<trigger.toStdString()<<" "<<(label_->text()).toStdString()<<std::endl;
	s<<(editor_->text()).toStdString()<<std::endl;
	return s;
}

//---------- GlobalValuesPrintElem

std::ostream& fttw::GlobalValuesPrintElem::printElem(std::ostream &s) const{
	if (labels_.size() == 0){
		std::cout<<"ERROR in "<<__FUNCTION__<<"(ostream&) const : line "<<__LINE__<<", file "<<__FILE__<<std::endl;
		std::cout<<"\tLabels were not declared in GlobalValuesPrintElem"<<std::endl;
		exit(-1);
	}
	if (labels_.size() != editors_.size()){
		std::cout<<"ERROR in "<<__FUNCTION__<<"(ostream&) const : line "<<__LINE__<<", file "<<__FILE__<<std::endl;
		std::cout<<"\tNumber of labels are not equal to the number of line editors in GlobalValuesPrintElem"<<std::endl;
		exit(-1);
	}

	s<<trigger.toStdString();
	size_t N = labels_.size();
	for(size_t i=0; i<N; ++i){
		s<<' '<<labels_[i]->text().toStdString();
	}
	s<<std::endl;

	s<<editors_[0]->text().toStdString();
	for(size_t i=1; i<N; ++i){
		s<<' '<<editors_[i]->text().toStdString();
	}
	s<<std::endl;
	return s;
}

//---------- CommentElement

std::ostream& fttw::CommentElement::printElem(std::ostream &s) const{
	s<<str_<<std::endl;
	return s;
}
