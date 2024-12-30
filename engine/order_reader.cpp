//
// Created by Muhammad chandra zulfikar on 30/12/24.
//
#include "order_reader.h"

#include <string_helper.h>

OrderReader::OrderReader(
     const std::string &brokers,
     const std::string &topic,
     const int64_t lastOffset,
     const bool &stopFlag
     ) : stopFlag(stopFlag), topic_(topic), consumer_([&]() {
          kafka::Properties props;
          props.put("bootstrap.servers", brokers);
          props.put("api.version.request", "false");
          props.put("enable.auto.commit", "false");
          props.put("allow.auto.create.topics", "true");
          return kafka::clients::consumer::KafkaConsumer(props);
      }()),
      lastOffset_(lastOffset)


{
     consumer_.subscribe({topic_});
     std::cout << "OrderReader Initialization." << '\n';
}

void OrderReader::setOffset(const int64_t offset)
{
     lastOffset_ = offset;
     kafka::TopicPartition const topicPartition(topic_, 0);
     consumer_.seek(topicPartition, lastOffset_);
}

PlaceOrderRequest OrderReader::fetchOrder() {

     try
    {
        for (const auto records = consumer_.poll(std::chrono::milliseconds(100)); const auto &record : records)
        {
            if (record.error())
            {
                std::cerr << "Error: " << record.toString() << '\n';
            }
            else
            {
                if (!records.empty())
                {
                    std::string const message = record.value().toString();
                    nlohmann::json json = nlohmann::json::parse(message);
                    json.at("offset") = record.offset();

                    consumer_.commitAsync();

                    return nlohmann::json::parse(json).get<PlaceOrderRequest>();;
                }
            }
        }
    }
    catch (const kafka::KafkaException &e)
    {
        std::cerr << "Kafka exception: " << e.what() << '\n';
    }
    return PlaceOrderRequest{};
}



