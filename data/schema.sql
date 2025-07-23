CREATE TABLE IF NOT EXISTS clients (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name TEXT NOT NULL,
  contact_email TEXT,
  phone TEXT,
  industry TEXT,
  status TEXT,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS sales_deals (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  client_id INTEGER,
  amount REAL,
  stage TEXT,
  close_date TEXT,
  sales_rep TEXT,
  FOREIGN KEY(client_id) REFERENCES clients(id)
);

CREATE TABLE IF NOT EXISTS support_tickets (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  client_id INTEGER,
  issue TEXT,
  status TEXT,
  created_at TEXT DEFAULT CURRENT_TIMESTAMP,
  resolved_at TEXT,
  FOREIGN KEY(client_id) REFERENCES clients(id)
);

CREATE TABLE IF NOT EXISTS employees (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name TEXT,
  role TEXT,
  department TEXT,
  email TEXT,
  hire_date TEXT,
  salary REAL
);

CREATE TABLE IF NOT EXISTS projects (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name TEXT,
  owner_id INTEGER,
  status TEXT,
  start_date TEXT,
  due_date TEXT,
  FOREIGN KEY(owner_id) REFERENCES employees(id)
);

CREATE TABLE IF NOT EXISTS tasks (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  project_id INTEGER,
  assigned_to INTEGER,
  description TEXT,
  status TEXT,
  due_date TEXT,
  FOREIGN KEY(project_id) REFERENCES projects(id),
  FOREIGN KEY(assigned_to) REFERENCES employees(id)
);
-- Add more tables for inventory, procurement, finance, compliance as needed
