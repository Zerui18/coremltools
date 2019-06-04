//
//  KNNValidatorTests.cpp
//  CoreML_framework
//
//  Created by Bill March on 10/12/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#include "MLModelTests.hpp"
#include "../src/Format.hpp"
#include "../src/Model.hpp"

#include "framework/TestUtils.hpp"

using namespace CoreML;

namespace CoreML { namespace KNNValidatorTests {

    void generateInterface(Specification::Model& m1);
    void addDataPoints(Specification::KNearestNeighborsClassifier* nnModel);
    void addIntLabels(Specification::KNearestNeighborsClassifier* nnModel);
    void addStringLabels(Specification::KNearestNeighborsClassifier* nnModel);

    void generateInterface(Specification::Model& m1) {

        m1.set_specificationversion(MLMODEL_SPECIFICATION_VERSION);
        Specification::ModelDescription* interface = m1.mutable_description();
        Specification::Metadata* metadata = interface->mutable_metadata();
        metadata->set_shortdescription(std::string("Testing nearest neighbor validator"));

        Specification::FeatureDescription *input = interface->add_input();
        Specification::FeatureType* inputType = new Specification::FeatureType;
        inputType->mutable_multiarraytype()->mutable_shape()->Add(4);
        inputType->mutable_multiarraytype()->set_datatype(::CoreML::Specification::ArrayFeatureType_ArrayDataType_FLOAT32);
        input->set_name("input");
        input->set_allocated_type(inputType);

        Specification::FeatureDescription *output = interface->add_output();
        Specification::FeatureType* outputType = new Specification::FeatureType;
        outputType->mutable_stringtype();
        output->set_name("output");
        output->set_allocated_type(outputType);
        
        interface->set_predictedfeaturename("output");

    }

    void addDataPoints(Specification::KNearestNeighborsClassifier* nnModel) {

        std::vector<float> point0 = {0, 0, 0, 0};
        std::vector<float> point0b = {0, 0.1f, 0, 0};

        std::vector<float> point1 = {1, 0, 0, 0};
        std::vector<float> point1b = {1, 0, 0.1f, 0};

        std::vector<float> point2 = {2.1f, 0, 0, 0};
        std::vector<float> point2b = {2.1f, 0, 0, 0.1f};

        std::vector<std::vector<float>> points = {point0, point1, point2, point0b, point1b, point2b};
        size_t pointCount = 6;

        auto *nnIndex = nnModel->mutable_nearestneighborsindex();
        nnIndex->set_numberofdimensions(4);

        for (size_t i = 0; i < pointCount; i++) {
            nnIndex->add_floatsamples();
            float *sample = ((std::vector<float>)points[i]).data();
            for (int j = 0; j < 4; j++) {
                nnIndex->mutable_floatsamples((int)i)->add_vector(sample[j]);
            }
        }

    }


    void addIntLabels(Specification::KNearestNeighborsClassifier* nnModel) {

        nnModel->mutable_int64classlabels()->add_vector(0);
        nnModel->mutable_int64classlabels()->add_vector(0);
        nnModel->mutable_int64classlabels()->add_vector(0);

        nnModel->mutable_int64classlabels()->add_vector(0);
        nnModel->mutable_int64classlabels()->add_vector(0);
        nnModel->mutable_int64classlabels()->add_vector(0);

    }

    void addStringLabels(Specification::KNearestNeighborsClassifier* nnModel) {

        nnModel->mutable_stringclasslabels()->add_vector(std::string("zero"));
        nnModel->mutable_stringclasslabels()->add_vector(std::string("zero"));
        nnModel->mutable_stringclasslabels()->add_vector(std::string("zero"));

        nnModel->mutable_stringclasslabels()->add_vector(std::string("zero"));
        nnModel->mutable_stringclasslabels()->add_vector(std::string("zero"));
        nnModel->mutable_stringclasslabels()->add_vector(std::string("zero"));

    }

}}

int testKNNValidatorNoPoints() {

    Specification::Model m1;

    KNNValidatorTests::generateInterface(m1);

    auto *nnModel = m1.mutable_knearestneighborsclassifier();
    nnModel->set_k(3);

    KNNValidatorTests::addStringLabels(nnModel);

    Result res = validate<MLModelType_kNearestNeighborsClassifier>(m1);
    ML_ASSERT_BAD(res);

    return 0;

}

int testKNNValidatorNoK() {

    Specification::Model m1;

    KNNValidatorTests::generateInterface(m1);

    auto *nnModel = m1.mutable_knearestneighborsclassifier();

    KNNValidatorTests::addDataPoints(nnModel);
    KNNValidatorTests::addStringLabels(nnModel);

    Result res = validate<MLModelType_kNearestNeighborsClassifier>(m1);
    ML_ASSERT_BAD(res);

    return 0;

}

int testKNNValidatorNoDimension() {

    Specification::Model m1;

    KNNValidatorTests::generateInterface(m1);

    auto *nnModel = m1.mutable_knearestneighborsclassifier();
    nnModel->set_k(3);

    KNNValidatorTests::addDataPoints(nnModel);
    KNNValidatorTests::addStringLabels(nnModel);

    auto *nnIndex = nnModel->mutable_nearestneighborsindex();
    nnIndex->set_numberofdimensions(0);

    Result res = validate<MLModelType_kNearestNeighborsClassifier>(m1);
    ML_ASSERT_BAD(res);
    return 0;

}

int testKNNValidatorNoLabels() {

    Specification::Model m1;

    KNNValidatorTests::generateInterface(m1);

    auto *nnModel = m1.mutable_knearestneighborsclassifier();
    nnModel->set_k(3);

    KNNValidatorTests::addDataPoints(nnModel);

    Result res = validate<MLModelType_kNearestNeighborsClassifier>(m1);
    ML_ASSERT_BAD(res);

    return 0;

}

int testKNNValidatorWrongNumberOfLabels() {

    Specification::Model m1;

    KNNValidatorTests::generateInterface(m1);

    auto *nnModel = m1.mutable_knearestneighborsclassifier();
    nnModel->set_k(3);

    KNNValidatorTests::addDataPoints(nnModel);
    KNNValidatorTests::addStringLabels(nnModel);
    nnModel->mutable_stringclasslabels()->add_vector(std::string("Idontwork"));

    Result res = validate<MLModelType_kNearestNeighborsClassifier>(m1);
    ML_ASSERT_BAD(res);

    return 0;

}

int testKNNValidatorNoIndex() {

    Specification::Model m1;

    KNNValidatorTests::generateInterface(m1);

    auto *knnClassifier = m1.mutable_knearestneighborsclassifier();
    knnClassifier->set_k(3);

    KNNValidatorTests::addDataPoints(knnClassifier);
    KNNValidatorTests::addStringLabels(knnClassifier);

    Result res = validate<MLModelType_kNearestNeighborsClassifier>(m1);
    ML_ASSERT_BAD(res);

    return 0;

}

int testKNNValidatorLinearIndex() {

    Specification::Model m1;

    KNNValidatorTests::generateInterface(m1);

    auto *knnClassifier = m1.mutable_knearestneighborsclassifier();
    knnClassifier->set_k(3);
    knnClassifier->mutable_uniformweighting();

    auto *nnIndex = knnClassifier->mutable_nearestneighborsindex();
    nnIndex->mutable_linearindex();
    nnIndex->mutable_squaredeuclideandistance();

    KNNValidatorTests::addDataPoints(knnClassifier);
    KNNValidatorTests::addStringLabels(knnClassifier);

    Result res = validate<MLModelType_kNearestNeighborsClassifier>(m1);
    ML_ASSERT_GOOD(res);

    return 0;

}

int testKNNValidatorSingleKdTreeIndex() {

    Specification::Model m1;

    KNNValidatorTests::generateInterface(m1);

    auto *knnClassifier = m1.mutable_knearestneighborsclassifier();
    knnClassifier->set_k(3);
    knnClassifier->mutable_uniformweighting();
    
    auto *nnIndex = knnClassifier->mutable_nearestneighborsindex();
    auto *kdTree = nnIndex->mutable_singlekdtreeindex();
    nnIndex->mutable_squaredeuclideandistance();

    KNNValidatorTests::addDataPoints(knnClassifier);
    KNNValidatorTests::addStringLabels(knnClassifier);

    // leaf size = 0 = bad.  We're requiring the user to specify a leaf size.
    Result res = validate<MLModelType_kNearestNeighborsClassifier>(m1);
    ML_ASSERT_BAD(res);

    // leafSize < 0 = bad
    kdTree->set_leafsize(-1);
    res = validate<MLModelType_kNearestNeighborsClassifier>(m1);
    ML_ASSERT_BAD(res);

    // leafSize > 0 = good
    kdTree->set_leafsize(30);
    res = validate<MLModelType_kNearestNeighborsClassifier>(m1);
    ML_ASSERT_GOOD(res);

    return 0;
}

int testKNNValidatorNoWeightingScheme() {

    Specification::Model m1;

    KNNValidatorTests::generateInterface(m1);

    auto *knnClassifier = m1.mutable_knearestneighborsclassifier();
    knnClassifier->set_k(3);

    auto *nnIndex = knnClassifier->mutable_nearestneighborsindex();
    nnIndex->mutable_linearindex();
    nnIndex->mutable_squaredeuclideandistance();

    KNNValidatorTests::addDataPoints(knnClassifier);
    KNNValidatorTests::addStringLabels(knnClassifier);

    Result res = validate<MLModelType_kNearestNeighborsClassifier>(m1);
    ML_ASSERT_BAD(res);

    return 0;

}

int testKNNValidatorNoDistanceFunction() {

    Specification::Model m1;

    KNNValidatorTests::generateInterface(m1);

    auto *knnClassifier = m1.mutable_knearestneighborsclassifier();
    knnClassifier->set_k(3);
    knnClassifier->mutable_uniformweighting();

    auto *nnIndex = knnClassifier->mutable_nearestneighborsindex();
    nnIndex->mutable_linearindex();

    KNNValidatorTests::addDataPoints(knnClassifier);
    KNNValidatorTests::addStringLabels(knnClassifier);

    Result res = validate<MLModelType_kNearestNeighborsClassifier>(m1);
    ML_ASSERT_BAD(res);

    return 0;

}

int testKNNValidatorGood() {

    Specification::Model m1;

    KNNValidatorTests::generateInterface(m1);

    auto *knnClassifier = m1.mutable_knearestneighborsclassifier();
    knnClassifier->set_k(3);
    knnClassifier->mutable_uniformweighting();

    auto *nnIndex = knnClassifier->mutable_nearestneighborsindex();
    nnIndex->mutable_squaredeuclideandistance();
    auto *kdTree = nnIndex->mutable_singlekdtreeindex();
    kdTree->set_leafsize(30);

    KNNValidatorTests::addDataPoints(knnClassifier);
    KNNValidatorTests::addStringLabels(knnClassifier);

    Result res = validate<MLModelType_kNearestNeighborsClassifier>(m1);
    ML_ASSERT_GOOD(res);

    return 0;

}

int testEmptyKNNValidationGood() {
    
    Specification::Model m1;
    
    KNNValidatorTests::generateInterface(m1);
    
    auto *knnClassifier = m1.mutable_knearestneighborsclassifier();
    knnClassifier->set_k(3);
    knnClassifier->mutable_uniformweighting();
    
    auto *nnIndex = knnClassifier->mutable_nearestneighborsindex();
    nnIndex->mutable_squaredeuclideandistance();
    auto *kdTree = nnIndex->mutable_singlekdtreeindex();
    kdTree->set_leafsize(30);
    
    // Validation should fail since we are not telling the label type.
    Result res = validate<MLModelType_kNearestNeighborsClassifier>(m1);
    ML_ASSERT_BAD(res);
    
    knnClassifier->set_defaultstringlabel("Default");
    
    // Validation should pass now.
    res = validate<MLModelType_kNearestNeighborsClassifier>(m1);
    ML_ASSERT_GOOD(res);
    
    knnClassifier->clear_defaultstringlabel();
    
    // Validation should fail since we are not telling the label type.
    res = validate<MLModelType_kNearestNeighborsClassifier>(m1);
    ML_ASSERT_BAD(res);

    return 0;    
}

int testLabelTypeMismatchTest() {
    
    Specification::Model m1;
    
    KNNValidatorTests::generateInterface(m1);
    
    auto *knnClassifier = m1.mutable_knearestneighborsclassifier();
    knnClassifier->set_k(3);
    knnClassifier->mutable_uniformweighting();
    
    auto *nnIndex = knnClassifier->mutable_nearestneighborsindex();
    nnIndex->mutable_squaredeuclideandistance();
    auto *kdTree = nnIndex->mutable_singlekdtreeindex();
    kdTree->set_leafsize(30);
    
    // Validation should fail since we are not telling the label type.
    Result res = validate<MLModelType_kNearestNeighborsClassifier>(m1);
    ML_ASSERT_BAD(res);
    
    knnClassifier->set_defaultstringlabel("Default");
    
    // Validation should pass now.
    res = validate<MLModelType_kNearestNeighborsClassifier>(m1);
    ML_ASSERT_GOOD(res);
    
    knnClassifier->mutable_int64classlabels();
    
    // Validation should fail due to mismatch in type
    res = validate<MLModelType_kNearestNeighborsClassifier>(m1);
    ML_ASSERT_BAD(res);
    
    return 0;
    
}
