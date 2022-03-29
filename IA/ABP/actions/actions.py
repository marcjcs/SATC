# This files contains your custom actions which can be used to run
# custom Python code.
#
# See this guide on how to implement these action:
# https://rasa.com/docs/rasa/custom-actions


# This is a simple example for a custom action which utters "Hello World!"

# from typing import Any, Text, Dict, List
#
# from rasa_sdk import Action, Tracker
# from rasa_sdk.executor import CollectingDispatcher
#
#
# class ActionHelloWorld(Action):
#
#     def name(self) -> Text:
#         return "action_hello_world"
#
#     def run(self, dispatcher: CollectingDispatcher,
#             tracker: Tracker,
#             domain: Dict[Text, Any]) -> List[Dict[Text, Any]]:
#
#         dispatcher.utter_message(text="Hello World!")
#
#         return []

import requests
from typing import Text, Dict, Any, List
from rasa_sdk import Action, Tracker
from rasa_sdk.events import SlotSet
from rasa_sdk.executor import CollectingDispatcher

class ActionCheckTotalDeathsByCountry(Action):
   def name(self) -> Text:
      return "action_check_total_deaths_by_country"

   def run(self,
           dispatcher: CollectingDispatcher,
           tracker: Tracker,
           domain: Dict[Text, Any]) -> List[Dict[Text, Any]]:

      country = tracker.get_slot('country')

      countries_url = 'https://api.covid19api.com/countries'
      countries = requests.get(countries_url)
      countries_data = countries.json()
      if not any(d['Country'] == country for d in countries_data):
        return [SlotSet("country_total_deaths", "Country not found, did you type it correctly?")]
      else:
        country_slug = next(item for item in countries_data if item['Country'] == country)['Slug']

      request_url = f'https://api.covid19api.com/total/dayone/country/{country_slug}'
      request_answer = requests.get(url = request_url)
      data = request_answer.json()

      return [SlotSet("country_total_deaths", 'Deaths since day one: {deaths}'.format(deaths=data[-1]['Deaths']) if data[-1] is not None else "Data not found")]

class ActionCheckTotalCasesByCountry(Action):
   def name(self) -> Text:
      return "action_check_total_cases_by_country"

   def run(self,
           dispatcher: CollectingDispatcher,
           tracker: Tracker,
           domain: Dict[Text, Any]) -> List[Dict[Text, Any]]:

      country = tracker.get_slot('country')

      countries_url = 'https://api.covid19api.com/countries'
      countries = requests.get(countries_url)
      countries_data = countries.json()
      if not any(d['Country'] == country for d in countries_data):
        return [SlotSet("country_total_cases", "Country not found, did you type it correctly?")]
      else:
        country_slug = next(item for item in countries_data if item['Country'] == country)['Slug']

      request_url = f'https://api.covid19api.com/total/dayone/country/{country_slug}'
      request_answer = requests.get(url = request_url)
      data = request_answer.json()

      return [SlotSet("country_total_cases", 'Cases since day one: {confirmed_cases}'.format(confirmed_cases=data[-1]['Confirmed']) if data[-1] is not None else "Data not found")]

class ActionCheckTotalRecoveredByCountry(Action):
   def name(self) -> Text:
      return "action_check_total_recovered_by_country"

   def run(self,
           dispatcher: CollectingDispatcher,
           tracker: Tracker,
           domain: Dict[Text, Any]) -> List[Dict[Text, Any]]:

      country = tracker.get_slot('country')

      countries_url = 'https://api.covid19api.com/countries'
      countries = requests.get(countries_url)
      countries_data = countries.json()
      if not any(d['Country'] == country for d in countries_data):
        return [SlotSet("country_total_recovered", "Country not found, did you type it correctly?")]
      else:
        country_slug = next(item for item in countries_data if item['Country'] == country)['Slug']

      request_url = f'https://api.covid19api.com/total/dayone/country/{country_slug}'
      request_answer = requests.get(url = request_url)
      data = request_answer.json()

      return [SlotSet("country_total_recovered", 'Recovered since day one: {recovered}'.format(recovered=data[-1]['Recovered']) if data[-1] is not None else "Data not found")]

class ActionCheckActiveCasesByCountry(Action):
   def name(self) -> Text:
      return "action_check_total_active_by_country"

   def run(self,
           dispatcher: CollectingDispatcher,
           tracker: Tracker,
           domain: Dict[Text, Any]) -> List[Dict[Text, Any]]:

      country = tracker.get_slot('country')

      countries_url = 'https://api.covid19api.com/countries'
      countries = requests.get(countries_url)
      countries_data = countries.json()
      if not any(d['Country'] == country for d in countries_data):
        return [SlotSet("country_total_active", "Country not found, did you type it correctly?")]
      else:
        country_slug = next(item for item in countries_data if item['Country'] == country)['Slug']

      request_url = f'https://api.covid19api.com/total/dayone/country/{country_slug}'
      request_answer = requests.get(url = request_url)
      data = request_answer.json()

      return [SlotSet("country_total_active", 'Active cases: {active_cases}'.format(active_cases=data[-1]['Active']) if data[-1] is not None else "Data not found")]

class ActionScheduleVaccination(Action):
   def name(self) -> Text:
      return "action_schedule_vaccination"

   def run(self,
           dispatcher: CollectingDispatcher,
           tracker: Tracker,
           domain: Dict[Text, Any]) -> List[Dict[Text, Any]]:

      name = tracker.get_slot('name')
      age = tracker.get_slot('age')
      comorbidities = tracker.get_slot('comorbidities')

      if (int(age) >= 40 and comorbidities == 'Yes'):
        return [SlotSet("schedule_result", f'{name}, you are scheduled to get a vaccination in May 8th, 2021, we will contact you through your telephone one day before the scheduled date.')]

      if (int(age) >= 60):
        return [SlotSet("schedule_result", f'{name}, you are scheduled to get a vaccination in May 7th, 2021, we will contact you through your telephone one day before the scheduled date.')]

      return [SlotSet("schedule_result", f'{name}, you cannot get the vaccine yet, vaccination is only open for people over 60 years old, or over 40 years old with comorbidities.')]
